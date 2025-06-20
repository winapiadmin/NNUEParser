#include "network.h"
namespace NNUEParser
{
    namespace Detail
    {

        // Read evaluation function parameters
        template <typename T>
        bool read_parameters(std::istream &stream, T &reference)
        {

            std::uint32_t header;
            header = read_little_endian<std::uint32_t>(stream);
            if (!stream || header != T::get_hash_value())
                return false;
            return reference.read_parameters(stream);
        }

        // Write evaluation function parameters
        template <typename T>
        bool write_parameters(std::ostream &stream, T &reference)
        {

            write_little_endian<std::uint32_t>(stream, T::get_hash_value());
            return reference.write_parameters(stream);
        }

    } // namespace Detail
    template <typename Arch, typename Transformer>
    void Network<Arch, Transformer>::initialize()
    {
        featureTransformer = make_unique_large_page<Transformer>();
        network = make_unique_aligned<Arch[]>(8);
    }
    // Read network header
    template <typename Arch, typename Transformer>
    bool Network<Arch, Transformer>::read_header(std::istream &stream,
                                                 std::uint32_t *hashValue,
                                                 std::string *desc) const
    {
        std::uint32_t version, size;

        version = read_little_endian<std::uint32_t>(stream);
        *hashValue = read_little_endian<std::uint32_t>(stream);
        size = read_little_endian<std::uint32_t>(stream);
        if (!stream || version != Version)
            return false;
        desc->resize(size);
        stream.read(&(*desc)[0], size);
        return !stream.fail();
    }
    template <typename Arch, typename Transformer>
    bool Network<Arch, Transformer>::read_parameters(std::istream &stream,
                                                     std::string &netDescription) const
    {
        std::uint32_t hashValue;
        if (!read_header(stream, &hashValue, &netDescription))
            return false;
        if (hashValue != Network::hash)
            return false;
        if (!Detail::read_parameters(stream, *featureTransformer))
            return false;
        for (std::size_t i = 0; i < LayerStacks; ++i)
        {
            if (!Detail::read_parameters(stream, network[i]))
                return false;
        }
        return stream && stream.peek() == std::ios::traits_type::eof();
    }
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
    template<typename Arch, typename Transformer>
    void Network<Arch, Transformer>::verify(std::string                                  evalfilePath,
                                            const std::function<void(std::string_view)>& f) const {
        if (evalfilePath.empty())
            evalfilePath = evalFile.defaultName;

        if (evalFile.current != evalfilePath)
        {
            if (f)
            {
                std::string msg1 =
                "Network evaluation parameters compatible with the engine must be available.";
                std::string msg2 = "The network file " + evalfilePath + " was not loaded successfully.";
                std::string msg3 = "The UCI option EvalFile might need to specify the full path, "
                                "including the directory name, to the network file.";
                std::string msg4 = "The default net can be downloaded from: "
                                "https://tests.stockfishchess.org/api/nn/"
                                + evalFile.defaultName;
                std::string msg5 = "The engine will be terminated now.";

                std::string msg = "ERROR: " + msg1 + '\n' + "ERROR: " + msg2 + '\n' + "ERROR: " + msg3
                                + '\n' + "ERROR: " + msg4 + '\n' + "ERROR: " + msg5 + '\n';

                f(msg);
            }

            exit(EXIT_FAILURE);
        }

        if (f)
        {
            size_t size = sizeof(*featureTransformer) + sizeof(Arch) * LayerStacks;
            f("NNUE evaluation using " + evalfilePath + " (" + std::to_string(size / (1024 * 1024))
            + "MiB, (" + std::to_string(featureTransformer->InputDimensions) + ", "
            + std::to_string(network[0].TransformedFeatureDimensions) + ", "
            + std::to_string(network[0].FC_0_OUTPUTS) + ", " + std::to_string(network[0].FC_1_OUTPUTS)
            + ", 1))");
        }
    }
    template <typename Arch, typename Transformer>
    void Network<Arch, Transformer>::load(const std::string &path)
    {
        initialize();
        std::ifstream stream(path, std::ios::binary);
        std::string description;

        if (read_parameters(stream, description))
        {
            evalFile.current        = path;
            evalFile.netDescription = description;
        }
        
    }
    template class Network<NetworkArchitecture<TransformedFeatureDimensionsBig, L2Big, L3Big>,
                           FeatureTransformer<TransformedFeatureDimensionsBig>>;

    template class Network<NetworkArchitecture<TransformedFeatureDimensionsSmall, L2Small, L3Small>,
                           FeatureTransformer<TransformedFeatureDimensionsSmall>>;
}