# Small network (SFNNv9) file format
## Header
| Field name | Data type    | Description                                     |
| ---------- | ------------ | ----------------------------------------------- |
| version    | `uint32_t` | Version of the NNUE |
| hash       | `uint32_t` | Hash of the file, compile-time const |
| str_size   | `uint32_t` | Size of the next string |
| description | `string` | Description of the network |

## Feature transformer
| Field name | Data type | Description | Size |
| - | - | - | - |
| hash | `uint32_t` | Hash of the whole feature transformer | 1 |
| biases | `int16_t` | Biases | 128 |
| weights | `int16_t` | Weights | 2883584 |
| psqtWeights | `int32_t` | PSQT weights | 180224 |

## Network architecture (8x the follows)

### Affine transform sparse input<InDims=128,OutDims=16>
| Field name | Data type | Description | Size |
| - | - | - | - |
| hash | `uint32_t` | Hash of the whole unit | 1 |
| biases | `int32_t` | Biases | 16 |
| weights | `int16_t` | Weights | 2048 |

### Square Clipped ReLU
Unused while read

### Clipped ReLU<16>
Nothing

### Affine transform<InDims=30,OutDims=32>
| Field name | Data type | Description | Size |
| - | - | - | - |
| hash | `uint32_t` | Hash of the whole unit | 1 |
| biases | `int32_t` | Biases | 32 |
| weights | `int8_t` | Weights | 1024 |

### ClippedReLU<32>
Nothing

### Affine transform<InDims=32,OutDims=1>

| Field name | Data type | Description | Size |
| - | - | - | - |
| hash | `uint32_t` | Hash of the whole unit | 1 |
| biases | `int32_t` | Biases | 32 |
| weights | `int8_t` | Weights | 32 |
