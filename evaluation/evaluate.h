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

#ifndef EVALUATE_H_INCLUDED
#define EVALUATE_H_INCLUDED
#include "types.h"
#define EvalFileDefaultNameBig "nn-1c0000000000.nnue"
#define EvalFileDefaultNameSmall "nn-37f18f62d772.nnue"
namespace NNUEParser {
struct Networks;
struct AccumulatorCaches;
class AccumulatorStack;
class Position;

Value evaluate(const Networks&    networks,
               const Position&    pos,
               AccumulatorStack&  accumulators,
               AccumulatorCaches& caches,
               int                optimism);
}

int to_cp(Value v, const NNUEParser::Position& pos);
#endif