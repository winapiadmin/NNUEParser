/*
  Stockfish, a UCI chess playing engine derived from Glaurung 2.1
  Copyright (C) 2004-2025 The Stockfish developers (see AUTHORS file)

  Stockfish is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Stockfish is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED
#include <cstdint>
#include <cassert>
namespace NNUEParser
{
    // clang-format off

    enum Color : int8_t
    {
        WHITE,
        BLACK,
        COLOR_NB = 2
    };
    constexpr Color operator~(Color c) { return Color(c ^ BLACK); }
    enum PieceType : std::int8_t {
        NO_PIECE_TYPE, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING,
        ALL_PIECES = 0,
        PIECE_TYPE_NB = 8
    };

    enum Piece : std::int8_t {
        NO_PIECE,
        W_PAWN = PAWN,     W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN, W_KING,
        B_PAWN = PAWN + 8, B_KNIGHT, B_BISHOP, B_ROOK, B_QUEEN, B_KING,
        PIECE_NB = 16
    };
    constexpr Piece make_piece(Color c, PieceType pt) { return Piece((c << 3) + pt); }
enum Square : int8_t {
    SQ_A1, SQ_B1, SQ_C1, SQ_D1, SQ_E1, SQ_F1, SQ_G1, SQ_H1,
    SQ_A2, SQ_B2, SQ_C2, SQ_D2, SQ_E2, SQ_F2, SQ_G2, SQ_H2,
    SQ_A3, SQ_B3, SQ_C3, SQ_D3, SQ_E3, SQ_F3, SQ_G3, SQ_H3,
    SQ_A4, SQ_B4, SQ_C4, SQ_D4, SQ_E4, SQ_F4, SQ_G4, SQ_H4,
    SQ_A5, SQ_B5, SQ_C5, SQ_D5, SQ_E5, SQ_F5, SQ_G5, SQ_H5,
    SQ_A6, SQ_B6, SQ_C6, SQ_D6, SQ_E6, SQ_F6, SQ_G6, SQ_H6,
    SQ_A7, SQ_B7, SQ_C7, SQ_D7, SQ_E7, SQ_F7, SQ_G7, SQ_H7,
    SQ_A8, SQ_B8, SQ_C8, SQ_D8, SQ_E8, SQ_F8, SQ_G8, SQ_H8,
    SQ_NONE,

    SQUARE_ZERO = 0,
    SQUARE_NB   = 64
};
    #define ENABLE_INCR_OPERATORS_ON(T) \
        constexpr T& operator++(T& d) { return d = T(int(d) + 1); } \
        constexpr T& operator--(T& d) { return d = T(int(d) - 1); }

ENABLE_INCR_OPERATORS_ON(PieceType)
ENABLE_INCR_OPERATORS_ON(Square)

    #undef ENABLE_INCR_OPERATORS_ON
    // clang-format on
    // Keep track of what a move changes on the board (used by NNUE)
    struct DirtyPiece
    {
        Piece pc;        // this is never allowed to be NO_PIECE
        Square from, to; // to should be SQ_NONE for promotions

        // if {add,remove}_sq is SQ_NONE, {add,remove}_pc is allowed to be
        // uninitialized
        // castling uses add_sq and remove_sq to remove and add the rook
        Square remove_sq, add_sq;
        Piece remove_pc, add_pc;
    };
    constexpr int MAX_PLY = 246;

    using Bitboard = uint64_t;

    // Returns the least significant bit in a non-zero bitboard.
    inline Square lsb(Bitboard b)
    {
        assert(b);

#if defined(__GNUC__) // GCC, Clang, ICX

        return Square(__builtin_ctzll(b));

#elif defined(_MSC_VER)
#ifdef _WIN64 // MSVC, WIN64

        unsigned long idx;
        _BitScanForward64(&idx, b);
        return Square(idx);

#else // MSVC, WIN32
        unsigned long idx;

        if (b & 0xffffffff)
        {
            _BitScanForward(&idx, int32_t(b));
            return Square(idx);
        }
        else
        {
            _BitScanForward(&idx, int32_t(b >> 32));
            return Square(idx + 32);
        }
#endif
#else // Compiler is neither GCC nor MSVC compatible
#error "Compiler not supported."
#endif
    }
    // Finds and clears the least significant bit in a non-zero bitboard.
    inline Square pop_lsb(Bitboard &b)
    {
        assert(b);
        const Square s = lsb(b);
        b &= b - 1;
        return s;
    }
    template <typename T, std::size_t MaxSize>
    class ValueList
    {

    public:
        std::size_t size() const { return size_; }
        void push_back(const T &value) { values_[size_++] = value; }
        const T *begin() const { return values_; }
        const T *end() const { return values_ + size_; }
        const T &operator[](int index) const { return values_[index]; }

    private:
        T values_[MaxSize];
        std::size_t size_ = 0;
    };
    using Value = int;

    constexpr Value VALUE_ZERO = 0;
    constexpr Value VALUE_DRAW = 0;
    constexpr Value VALUE_NONE = 32002;
    constexpr Value VALUE_INFINITE = 32001;

    constexpr Value VALUE_MATE = 32000;
    constexpr Value VALUE_MATE_IN_MAX_PLY = VALUE_MATE - MAX_PLY;
    constexpr Value VALUE_MATED_IN_MAX_PLY = -VALUE_MATE_IN_MAX_PLY;

    constexpr Value VALUE_TB = VALUE_MATE_IN_MAX_PLY - 1;
    constexpr Value VALUE_TB_WIN_IN_MAX_PLY = VALUE_TB - MAX_PLY;
    constexpr Value VALUE_TB_LOSS_IN_MAX_PLY = -VALUE_TB_WIN_IN_MAX_PLY;
}
#endif
