# NNUEParser
Stockfish's HalfKA_v2_hm (SFNNv9, can be adapted until SFNNv5) NNUE file structure, extra: evaluation function (see official-stockfish/nnue-pytorch)

To compile the evaluation: (it's pretty crappy and have a wrapper)
- `clang++ evaluation/features/half_ka_v2_hm.cpp evaluation/accumulator.cpp evaluation/memory.cpp evaluation/network.cpp evaluation/main.cpp evaluation/position.cpp -std=c++17`
