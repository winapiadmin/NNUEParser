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

#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED
#include <fstream>
#include <tuple>

#include "architecture.h"
#include "common.h"
#include "feature_transformer.h"
#include "memory.h"
namespace NNUEParser {

enum class EmbeddedNNUEType {
    BIG,
    SMALL,
};
using Value         = int;
using NetworkOutput = std::tuple<Value, Value>;
struct EvalFile {
    // Default net name, will use one of the EvalFileDefaultName* macros defined
    // in evaluate.h
    std::string defaultName;
    // Selected net name, either via uci option or default
    std::string current;
    // Net description extracted from the net file
    std::string netDescription;
};
template<typename Arch, typename Transformer>
class Network {
    static constexpr IndexType FTDimensions = Arch::TransformedFeatureDimensions;

   public:
    void load(const std::string& path);

    NetworkOutput evaluate(const Position&                         pos,
                           AccumulatorStack&                       accumulatorStack,
                           AccumulatorCaches::Cache<FTDimensions>* cache) const;
    void verify(std::string evalfilePath, const std::function<void(std::string_view)>&) const;

   private:
    void initialize();
    // Input feature converter
    LargePagePtr<Transformer> featureTransformer;

    // Evaluation function
    AlignedPtr<Arch[]>             network;
    bool                           read_header(std::istream&, std::uint32_t*, std::string*) const;
    bool                           read_parameters(std::istream&, std::string&) const;
    static constexpr std::uint32_t hash = Transformer::get_hash_value() ^ Arch::get_hash_value();
    EvalFile                       evalFile;
    EmbeddedNNUEType               embeddedType;
    template<IndexType Size>
    friend struct AccumulatorCaches::Cache;

    friend class AccumulatorStack;
};

// Definitions of the network types
using SmallFeatureTransformer = FeatureTransformer<TransformedFeatureDimensionsSmall>;
using SmallNetworkArchitecture =
  NetworkArchitecture<TransformedFeatureDimensionsSmall, L2Small, L3Small>;

using BigFeatureTransformer  = FeatureTransformer<TransformedFeatureDimensionsBig>;
using BigNetworkArchitecture = NetworkArchitecture<TransformedFeatureDimensionsBig, L2Big, L3Big>;

using NetworkBig   = Network<BigNetworkArchitecture, BigFeatureTransformer>;
using NetworkSmall = Network<SmallNetworkArchitecture, SmallFeatureTransformer>;

struct Networks {
    Networks(NetworkBig&& nB, NetworkSmall&& nS) :
        big(std::move(nB)),
        small(std::move(nS)) {}
    Networks();
    NetworkBig   big;
    NetworkSmall small;
};
}  // namespace NNUEParser
#endif
