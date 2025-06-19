# Stockfish's NNUE file format (feature set: HalfKA_v2_hm)

**This file have the full evaluation with file format, including clipped ReLU**
Attributes used here:
- `[hash]` - hash values to compare against the Stockfish constexpr `T::get_hash_value()`
- `[biases]`
- `[weights]`
- `[psqtWeights]`
- `[size]` - size of the description
- `[description]` - the description itself
- `[[little-endian]]` (of course as it's LEB128, Little Endian base 128)
- `[[array]]` - A LEB128 array, but not a normal array (see nnue_common.h)

```
3 uint32_t [version, hash, size] [[little_endian]]
1 string(size) [description]
1 uint32_t [hash] [[little_endian]]
3072 int16_t [biases] [[little_endian]]
69206016 int16_t [weights] [[little_endian]]
180224 int16_t [psqtWeights] [[little-endian]]

1 uint32_t [hash] [[little_endian]]
16 int16_t [biases] [[little-endian]]
41952x1 int16_t [weights] [[little-endian]] [[array]]
0 none [clippedrelu]
32 int16_t [biases] [[little-endian]]
1024x1 int16_t [weights] [[little-endian]] [[array]]
0 none [clippedrelu]
1 int16_t [biases] [[little-endian]]
480x1 int16_t [weights] [[little-endian]] [[array]]

1 uint32_t [hash] [[little_endian]]
16 int16_t [biases] [[little-endian]]
41952x1 int16_t [weights] [[little-endian]] [[array]]
0 none [clippedrelu]
32 int16_t [biases] [[little-endian]]
1024x1 int16_t [weights] [[little-endian]] [[array]
0 none [clippedrelu]
1 int16_t [biases] [[little-endian]]
480x1 int16_t [weights] [[little-endian]] [[array]]

1 uint32_t [hash] [[little_endian]]
16 int16_t [biases] [[little-endian]]
41952x1 int16_t [weights] [[little-endian]] [[array]]
0 none [clippedrelu]
32 int16_t [biases] [[little-endian]]
1024x1 int16_t [weights] [[little-endian]] [[array]]
0 none [clippedrelu]
1 int16_t [biases] [[little-endian]]
480x1 int16_t [weights] [[little-endian]] [[array]]

1 uint32_t [hash] [[little_endian]]
16 int16_t [biases] [[little-endian]]
41952x1 int16_t [weights] [[little-endian]] [[array]]
0 none [clippedrelu]
32 int16_t [biases] [[little-endian]]
1024x1 int16_t [weights] [[little-endian]] [[array]]
0 none [clippedrelu]
1 int16_t [biases] [[little-endian]]
480x1 int16_t [weights] [[little-endian]] [[array]]

1 uint32_t [hash] [[little_endian]]
16 int16_t [biases] [[little-endian]]
41952x1 int16_t [weights] [[little-endian]] [[array]]
0 none [clippedrelu]
32 int16_t [biases] [[little-endian]]
1024x1 int16_t [weights] [[little-endian]] [[array]]
0 none [clippedrelu]
1 int16_t [biases] [[little-endian]]
480x1 int16_t [weights] [[little-endian]] [[array]]

1 uint32_t [hash] [[little_endian]]
16 int16_t [biases] [[little-endian]]
41952x1 int16_t [weights] [[little-endian]] [[array]]
0 none [clippedrelu]
32 int16_t [biases] [[little-endian]]
1024x1 int16_t [weights] [[little-endian]] [[array]]
0 none [clippedrelu]
1 int16_t [biases] [[little-endian]]
480x1 int16_t [weights] [[little-endian]] [[array]]

1 uint32_t [hash] [[little_endian]]
16 int16_t [biases] [[little-endian]]
41952x1 int16_t [weights] [[little-endian]] [[array]]
0 none [clippedrelu]
32 int16_t [biases] [[little-endian]]
1024x1 int16_t [weights] [[little-endian]] [[array]]
0 none [clippedrelu]
1 int16_t [biases] [[little-endian]]
480x1 int16_t [weights] [[little-endian]] [[array]]

1 uint32_t [hash] [[little_endian]]
16 int16_t [biases] [[little-endian]]
41952x1 int16_t [weights] [[little-endian]] [[array]]
0 none [clippedrelu]
32 int16_t [biases] [[little-endian]]
1024x1 int16_t [weights] [[little-endian]] [[array]]
0 none [clippedrelu]
1 int16_t [biases] [[little-endian]]
480x1 int16_t [weights] [[little-endian]] [[array]]
```

To evaluate:
```
template<typename Arch, typename Transformer>
NetworkOutput
Network<Arch, Transformer>::evaluate(const Position&                         pos,
                                     AccumulatorStack&                       accumulatorStack,
                                     AccumulatorCaches::Cache<FTDimensions>* cache) const {

    constexpr uint64_t alignment = CacheLineSize;

    alignas(alignment)
      TransformedFeatureType transformedFeatures[FeatureTransformer<FTDimensions>::BufferSize];

    ASSERT_ALIGNED(transformedFeatures, alignment);

    const int  bucket = (pos.count<ALL_PIECES>() - 1) / 4;
    const auto psqt =
      featureTransformer->transform(pos, accumulatorStack, cache, transformedFeatures, bucket);
    const auto positional = network[bucket].propagate(transformedFeatures);
    return {static_cast<Value>(psqt / OutputScale), static_cast<Value>(positional / OutputScale)};
}
```
