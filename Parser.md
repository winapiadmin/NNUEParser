# Stockfish's NNUE file format (feature set: HalfKA_v2_hm)

**NOTE: THIS IS FILE FORMAT, NOT INCLUDES CLIPPED RELU**

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
32 int16_t [biases] [[little-endian]]
1024x1 int16_t [weights] [[little-endian]] [[array]]
1 int16_t [biases] [[little-endian]]
480x1 int16_t [weights] [[little-endian]] [[array]]

1 uint32_t [hash] [[little_endian]]
16 int16_t [biases] [[little-endian]]
41952x1 int16_t [weights] [[little-endian]] [[array]]
32 int16_t [biases] [[little-endian]]
1024x1 int16_t [weights] [[little-endian]] [[array]
1 int16_t [biases] [[little-endian]]
480x1 int16_t [weights] [[little-endian]] [[array]]

1 uint32_t [hash] [[little_endian]]
16 int16_t [biases] [[little-endian]]
41952x1 int16_t [weights] [[little-endian]] [[array]]
32 int16_t [biases] [[little-endian]]
1024x1 int16_t [weights] [[little-endian]] [[array]]
1 int16_t [biases] [[little-endian]]
480x1 int16_t [weights] [[little-endian]] [[array]]

1 uint32_t [hash] [[little_endian]]
16 int16_t [biases] [[little-endian]]
41952x1 int16_t [weights] [[little-endian]] [[array]]
32 int16_t [biases] [[little-endian]]
1024x1 int16_t [weights] [[little-endian]] [[array]]
1 int16_t [biases] [[little-endian]]
480x1 int16_t [weights] [[little-endian]] [[array]]

1 uint32_t [hash] [[little_endian]]
16 int16_t [biases] [[little-endian]]
41952x1 int16_t [weights] [[little-endian]] [[array]]
32 int16_t [biases] [[little-endian]]
1024x1 int16_t [weights] [[little-endian]] [[array]]
1 int16_t [biases] [[little-endian]]
480x1 int16_t [weights] [[little-endian]] [[array]]

1 uint32_t [hash] [[little_endian]]
16 int16_t [biases] [[little-endian]]
41952x1 int16_t [weights] [[little-endian]] [[array]]
32 int16_t [biases] [[little-endian]]
1024x1 int16_t [weights] [[little-endian]] [[array]]
1 int16_t [biases] [[little-endian]]
480x1 int16_t [weights] [[little-endian]] [[array]]

1 uint32_t [hash] [[little_endian]]
16 int16_t [biases] [[little-endian]]
41952x1 int16_t [weights] [[little-endian]] [[array]]
32 int16_t [biases] [[little-endian]]
1024x1 int16_t [weights] [[little-endian]] [[array]]
1 int16_t [biases] [[little-endian]]
480x1 int16_t [weights] [[little-endian]] [[array]]

1 uint32_t [hash] [[little_endian]]
16 int16_t [biases] [[little-endian]]
41952x1 int16_t [weights] [[little-endian]] [[array]]
32 int16_t [biases] [[little-endian]]
1024x1 int16_t [weights] [[little-endian]] [[array]]
1 int16_t [biases] [[little-endian]]
480x1 int16_t [weights] [[little-endian]] [[array]]
```
