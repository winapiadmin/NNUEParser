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
// A wrapper header, you can implement one yourself
#ifndef POSITION_H_INCLUDED
#define POSITION_H_INCLUDED
#include "chess.hpp"
#include "types.h"
namespace NNUEParser{
  using _Move=chess::Move;
  using _Board=chess::Board;
    class Position{
        public:
        Position(const _Board& board);
        inline Bitboard pieces(Color c) const;
        inline Bitboard pieces(Color c, PieceType pt) const;
        inline Bitboard pieces(PieceType pt) const;
        inline Bitboard pieces() const;
        template<PieceType Pt>
        inline int count() const;
        template<PieceType Pt>
        inline int count(Color c) const;
        template<PieceType Pt>
        inline Square square(Color c) const {
            assert(count<Pt>(c) == 1);
            return lsb(pieces(c, Pt));
        }
        inline Color side_to_move() const;
        inline Piece piece_on(const Square sq) const;
        inline int non_pawn_material() const;
        inline int non_pawn_material(Color c) const;
        inline int rule50_count() const;
        DirtyPiece do_move(_Move m);
        void undo_move(_Move m);
        void do_null_move();
        void undo_null_move();
        _Board b; // exposure to generate legal moves
        private:
        int npm[2]={0,0}; // incremental non pawn material calculation
    };
}
#endif
