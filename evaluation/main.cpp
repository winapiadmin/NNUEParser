#include "network.h"
#include "position.h"
#include "chess.hpp"
#include <cassert>
#include <iostream>
#include <cmath>
#include <chrono>
namespace NNUEParser{
// Returns a static, purely materialistic evaluation of the position from
// the point of view of the side to move. It can be divided by PawnValue to get
// an approximation of the material advantage on the board in terms of pawns.
int simple_eval(const Position& pos) {
    Color c = pos.side_to_move();
    return 208 * (pos.count<PAWN>(c) - pos.count<PAWN>(~c))
         + (pos.non_pawn_material(c) - pos.non_pawn_material(~c));
}

bool use_smallnet(const Position& pos) { return std::abs(simple_eval(pos)) > 962; }
// Evaluate is the evaluator for the outer world. It returns a static evaluation
// of the position from the point of view of the side to move.
Value       evaluate(const Networks&                networks,
                     const Position&                pos,
                     AccumulatorStack&              accumulators,
                     AccumulatorCaches&             caches,
                     int                            optimism) {
    bool smallNet           = use_smallnet(pos);
    auto [psqt, positional] = smallNet ? networks.small.evaluate(pos, accumulators, &caches.small)
                                       : networks.big.evaluate(pos, accumulators, &caches.big);

    Value nnue = (125 * psqt + 131 * positional) / 128;

    // Re-evaluate the position when higher eval accuracy is worth the time spent
    if (smallNet && (std::abs(nnue) < 236))
    {
        std::tie(psqt, positional) = networks.big.evaluate(pos, accumulators, &caches.big);
        nnue                       = (125 * psqt + 131 * positional) / 128;
        smallNet                   = false;
    }

    // Blend optimism and eval with nnue complexity
    int nnueComplexity = std::abs(psqt - positional);
    optimism += optimism * nnueComplexity / 468;
    nnue -= nnue * nnueComplexity / 18000;

    int material = 535 * pos.count<PAWN>() + pos.non_pawn_material();
    int v        = (nnue * (77777 + material) + optimism * (7777 + material)) / 77777;

    // Damp down the evaluation linearly when shuffling
    v -= v * pos.rule50_count() / 212;

    // Guarantee evaluation does not hit the tablebase range
    v = std::clamp(v, VALUE_TB_LOSS_IN_MAX_PLY + 1, VALUE_TB_WIN_IN_MAX_PLY - 1);

    return v;
}
}
#define EvalFileDefaultNameBig "nn-1c0000000000.nnue"
#define EvalFileDefaultNameSmall "nn-37f18f62d772.nnue"
void onVerify(std::string_view sv){
    std::cout<<sv<<'\n';
}
struct WinRateParams {
    double a;
    double b;
};

WinRateParams win_rate_params(const NNUEParser::Position& pos) {

    int material = pos.count<NNUEParser::PAWN>() + 3 * pos.count<NNUEParser::KNIGHT>() + 3 * pos.count<NNUEParser::BISHOP>()
                 + 5 * pos.count<NNUEParser::ROOK>() + 9 * pos.count<NNUEParser::QUEEN>();

    // The fitted model only uses data for material counts in [17, 78], and is anchored at count 58.
    double m = std::clamp(material, 17, 78) / 58.0;

    // Return a = p_a(material) and b = p_b(material), see github.com/official-stockfish/WDL_model
    constexpr double as[] = {-13.50030198, 40.92780883, -36.82753545, 386.83004070};
    constexpr double bs[] = {96.53354896, -165.79058388, 90.89679019, 49.29561889};

    double a = (((as[0] * m + as[1]) * m + as[2]) * m) + as[3];
    double b = (((bs[0] * m + bs[1]) * m + bs[2]) * m) + bs[3];

    return {a, b};
}
// Turns a Value to an integer centipawn number,
// without treatment of mate and similar special scores.
int to_cp(NNUEParser::Value v, const NNUEParser::Position& pos) {

    // In general, the score can be defined via the WDL as
    // (log(1/L - 1) - log(1/W - 1)) / (log(1/L - 1) + log(1/W - 1)).
    // Based on our win_rate_model, this simply yields v / a.

    auto [a, b] = win_rate_params(pos);

    return std::round(100 * int(v) / a);
}
int main(){
    NNUEParser::NetworkBig nBig;
    NNUEParser::NetworkSmall nSmall;
    nBig.load(EvalFileDefaultNameBig);
    nBig.verify(EvalFileDefaultNameBig, onVerify);
    nSmall.load(EvalFileDefaultNameSmall);
    nSmall.verify(EvalFileDefaultNameSmall, onVerify);
    NNUEParser::Networks nn(std::move(nBig), std::move(nSmall));
    NNUEParser::AccumulatorStack stack;
    NNUEParser::AccumulatorCaches cache(nn);
    chess::Board b;
    NNUEParser::Position pos(b);

    std::cout << "DISCLAIMER: Using this NNUE model and output requires converting to centipawns\n";

    constexpr int N = 10000000; // Number of evaluations to benchmark

    auto start = std::chrono::high_resolution_clock::now();
    int total = 0;
    for (int i = 0; i < N; ++i) {
        total = to_cp(NNUEParser::evaluate(nn, pos, stack, cache, 0), pos);
    }
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << "Total time: " << elapsed.count() << " ms\n";
    std::cout << "Average time per evaluation: " << (elapsed.count() / N) << " ms, "<<(1.0/(elapsed.count()/N))<<" eval/s\n";
    std::cout << "Final dummy value: " << total << "\n";
}