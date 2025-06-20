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

#ifndef POSITION_H_INCLUDED
#define POSITION_H_INCLUDED
#include "chess.hpp"
#include "types.h"
namespace NNUEParser{
    class Position{
        public:
        Position(const chess::Board& board):b(board){}
        inline Bitboard pieces(Color c) const {
            // WHITE and BLACK common, NONE not handled here
            return b.us(c).getBits();
        }
        // some incompatible values:
        // disservin/chess-library: PAWN=0
        // official-stockfish/Stockfish: PAWN=1
        // so subtract 1
        inline Bitboard pieces(Color c, PieceType pt) const{
            switch(pt){
                case ALL_PIECES:
                    return pieces(c);
                default:
                    return b.pieces(chess::PieceType(static_cast<chess::PieceType::underlying>(pt-1)), c).getBits();
            }
        }
        inline Bitboard pieces(PieceType pt) const{
            switch(pt){
                case ALL_PIECES:
                    return b.occ().getBits();
                default:
                    return b.pieces(chess::PieceType(static_cast<chess::PieceType::underlying>(pt-1))).getBits();
            }
        }
        inline Bitboard pieces() const{
            return b.occ().getBits();
        }
        template<PieceType Pt>
        inline int count() const{
            return chess::Bitboard(pieces(Pt)).count();
        }
        template<PieceType Pt>
        inline int count(Color c) const{
            return chess::Bitboard(pieces(c,Pt)).count();
        }
        template<PieceType Pt>
        inline Square square(Color c) const {
            assert(count<Pt>(c) == 1);
            return lsb(pieces(c, Pt));
        }
        inline Color side_to_move() const{
            return (Color)static_cast<int>(b.sideToMove());
        }
        inline Piece piece_on(const Square sq) const{
            return to_engine_piece(b.at(sq).internal());
        }
        inline int non_pawn_material() const {
            return count<KNIGHT>()*781+count<BISHOP>()*825+count<ROOK>()*1276+count<QUEEN>()*2538;
        }
        inline int non_pawn_material(Color c) const {
            return count<KNIGHT>(c)*781+count<BISHOP>(c)*825+count<ROOK>(c)*1276+count<QUEEN>(c)*2538;
        }
        inline int rule50_count() const{
            return b.halfMoveClock();
        }
        private:
        chess::Board b;
        inline Piece to_engine_piece(chess::Piece::underlying u) const{
            switch (u) {
                case chess::Piece::underlying::WHITEPAWN: return W_PAWN;
                case chess::Piece::underlying::WHITEKNIGHT: return W_KNIGHT;
                case chess::Piece::underlying::WHITEBISHOP: return W_BISHOP;
                case chess::Piece::underlying::WHITEROOK: return W_ROOK;
                case chess::Piece::underlying::WHITEQUEEN: return W_QUEEN;
                case chess::Piece::underlying::WHITEKING: return W_KING;
                case chess::Piece::underlying::BLACKPAWN: return B_PAWN;
                case chess::Piece::underlying::BLACKKNIGHT: return B_KNIGHT;
                case chess::Piece::underlying::BLACKBISHOP: return B_BISHOP;
                case chess::Piece::underlying::BLACKROOK: return B_ROOK;
                case chess::Piece::underlying::BLACKQUEEN: return B_QUEEN;
                case chess::Piece::underlying::BLACKKING: return B_KING;
                default: return NO_PIECE;
            }
        }
    };
}
#endif