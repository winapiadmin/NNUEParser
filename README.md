# NNUEParser
Stockfish's HalfKA_v2_hm NNUE file structure, extra: parser and evaluation function (see official-stockfish/nnue-pytorch)

To compile the evaluator: (it's pretty crappy and have a wrapper)
- `clang++ NNUEParser/evaluation/features/half_ka_v2_hm.cpp NNUEParser/evaluation/accumulator.cpp NNUEParser/evaluation/memory.cpp NNUEParser/evaluation/network.cpp NNUEParser/evaluation/main.cpp -std=c++17`
