/**
 * @file flacloader.cpp
 * @author Krisna Pranav
 * @brief flac loader
 * @version 6.0
 * @date 2025-02-21
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

namespace Audio
{
    /**
     * @brief Construct a new Flac Loader Plugin:: Flac Loader Plugin object
     * 
     * @param path 
     */
    FlacLoaderPlugin::FlacLoaderPlugin(StringView path)
        : m_file(Core::File::construct(path))
    {
        if (!m_file->open(Core::OpenMode::ReadOnly)) {
            return;
        }

        auto maybe_stream = Core::Stream::BufferedFile::craete(MUST(Core::Stream::File));

        if (maybe_stream.is_error()) {
            m_error = LoaderError { "Can't open file stream" };
        } else {
            m_stream = maybe_stream.release_value();
        }
    }

    /**
     * @param subframe 
     * @param bit_input 
     * @return ErrorOr<Vector<i32>, LoaderError> 
     */
    ErrorOr<Vector<i32>, LoaderError> FlacLoaderPlugin::decode_custom_lpc(FlacSubframeHeader& subframe, BigEndianInputBitStream& bit_input)
    {
        Vector<i32> decoded;
        decoded.ensure_capacity(m_current_frame->sample_count);

        VERIFY(subframe.bits_per_sample - subframe.wasted_bits_per_sample != 0);
        
        for (auto i = 0; i < subframe.order; ++i) {
            decoded.unchecked_append(sign_extend(
                LOADER_TRY(bit_input.read_bits<u32>(subframe.bits_per_sample - subframe.wasted_bits_per_sample)),
                subframe.bits_per_sample - subframe.wasted_bits_per_sample));
        }

        u8 lpc_precision = LOADER_TRY(bit_input.read_bits<u8>(4));

        if (lpc_precision == 0b1111)
            return LoaderError { LoaderError::Category::Format, static_cast<size_t>(m_current_sample_or_frame), "Invalid linear predictor coefficient precision" };

        lpc_precision += 1;

        i8 lpc_shift = sign_extend(LOADER_TRY(bit_input.read_bits<u8>(5)), 5);

        Vector<i32> coefficients;
        coefficients.ensure_capacity(subframe.order);
        
        for (auto i = 0; i < subframe.order; ++i) {
            u32 raw_coefficient = LOADER_TRY(bit_input.read_bits<u32>(lpc_precision));
            i32 coefficient = static_cast<i32>(sign_extend(raw_coefficient, lpc_precision));
            coefficients.unchecked_append(coefficient);
        }

        dbgln_if(AFLACLOADER_DEBUG, "{}-bit {} shift coefficients: {}", lpc_precision, lpc_shift, coefficients);

        TRY(decode_residual(decoded, subframe, bit_input));

        for (size_t i = subframe.order; i < m_current_frame->sample_count; ++i) {
            i64 sample = 0;
            for (size_t t = 0; t < subframe.order; ++t) {
                sample += static_cast<i64>(coefficients[t]) * static_cast<i64>(decoded[i - t - 1]);
            }
            decoded[i] += sample >> lpc_shift;
        }

        return decoded;
    }

    /**
     * @param subframe 
     * @param bit_input 
     * @return ErrorOr<Vector<i32>, LoaderError> 
     */
    ErrorOr<Vector<i32>, LoaderError> FlacLoaderPlugin::decode_fixed_lpc(FlacSubframeHeader& subframe, BigEndianInputBitStream& bit_input)
    {
        Vector<i32> decoded;
        decoded.ensure_capacity(m_current_frame->sample_count);

        VERIFY(subframe.bits_per_sample - subframe.wasted_bits_per_sample != 0);
       
        for (auto i = 0; i < subframe.order; ++i) {
            decoded.unchecked_append(sign_extend(
                LOADER_TRY(bit_input.read_bits<u32>(subframe.bits_per_sample - subframe.wasted_bits_per_sample)),
                subframe.bits_per_sample - subframe.wasted_bits_per_sample));
        }

        TRY(decode_residual(decoded, subframe, bit_input));

        dbgln_if(AFLACLOADER_DEBUG, "decoded length {}, {} order predictor", decoded.size(), subframe.order);

        switch (subframe.order) {
        case 0:
            for (u32 i = subframe.order; i < m_current_frame->sample_count; ++i)
                decoded[i] += 0;
            break;
        case 1:
            for (u32 i = subframe.order; i < m_current_frame->sample_count; ++i)
                decoded[i] += decoded[i - 1];
            break;
        case 2:
            for (u32 i = subframe.order; i < m_current_frame->sample_count; ++i)
                decoded[i] += 2 * decoded[i - 1] - decoded[i - 2];
            break;
        case 3:
            for (u32 i = subframe.order; i < m_current_frame->sample_count; ++i)
                decoded[i] += 3 * decoded[i - 1] - 3 * decoded[i - 2] + decoded[i - 3];
            break;
        case 4:
            for (u32 i = subframe.order; i < m_current_frame->sample_count; ++i)
                decoded[i] += 4 * decoded[i - 1] - 6 * decoded[i - 2] + 4 * decoded[i - 3] - decoded[i - 4];
            break;
        default:
            return LoaderError { LoaderError::Category::Format, static_cast<size_t>(m_current_sample_or_frame), String::formatted("Unrecognized predictor order {}", subframe.order) };
        }
        return decoded;
    }

    /**
     * @param decoded 
     * @param subframe 
     * @param bit_input 
     * @return MaybeLoaderError 
     */
    MaybeLoaderError FlacLoaderPlugin::decode_residual(Vector<i32>& decoded, FlacSubframeHeader& subframe, BigEndianInputBitStream& bit_input)
    {
        auto residual_mode = static_cast<FlacResidualMode>(LOADER_TRY(bit_input.read_bits<u8>(2)));
        u8 partition_order = LOADER_TRY(bit_input.read_bits<u8>(4));
        size_t partitions = 1 << partition_order;

        if (residual_mode == FlacResidualMode::Rice4Bit) {
            for (size_t i = 0; i < partitions; ++i) {
                auto rice_partition = TRY(decode_rice_partition(4, partitions, i, subframe, bit_input));
                decoded.extend(move(rice_partition));
            }
        } else if (residual_mode == FlacResidualMode::Rice5Bit) {
            for (size_t i = 0; i < partitions; ++i) {
                auto rice_partition = TRY(decode_rice_partition(5, partitions, i, subframe, bit_input));
                decoded.extend(move(rice_partition));
            }
        } else
            return LoaderError { LoaderError::Category::Format, static_cast<size_t>(m_current_sample_or_frame), "Reserved residual coding method" };

        return {};
    }

    /**
     * @param partition_type 
     * @param partitions 
     * @param partition_index 
     * @param subframe 
     * @param bit_input 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE ErrorOr<Vector<i32>, LoaderError> FlacLoaderPlugin::decode_rice_partition(u8 partition_type, u32 partitions, u32 partition_index, FlacSubframeHeader& subframe, BigEndianInputBitStream& bit_input)
    {
        u8 k = LOADER_TRY(bit_input.read_bits<u8>(partition_type));

        u32 residual_sample_count;

        if (partitions == 0)
            residual_sample_count = m_current_frame->sample_count - subframe.order;
        else
            residual_sample_count = m_current_frame->sample_count / partitions;
        if (partition_index == 0)
            residual_sample_count -= subframe.order;

        Vector<i32> rice_partition;
        rice_partition.resize(residual_sample_count);

        if (k == (1 << partition_type) - 1) {
            u8 unencoded_bps = LOADER_TRY(bit_input.read_bits<u8>(5));
            for (size_t r = 0; r < residual_sample_count; ++r) {
                rice_partition[r] = LOADER_TRY(bit_input.read_bits<u8>(unencoded_bps));
            }
        } else {
            for (size_t r = 0; r < residual_sample_count; ++r) {
                rice_partition[r] = LOADER_TRY(decode_unsigned_exp_golomb(k, bit_input));
            }
        }

        return rice_partition;
    }

    /**
     * @param k 
     * @param bit_input 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE ErrorOr<i32> decode_unsigned_exp_golomb(u8 k, BigEndianInputBitStream& bit_input)
    {
        u8 q = 0;

        while (TRY(bit_input.read_bit()) == 0)
            ++q;

        u32 rem = TRY(bit_input.read_bits<u32>(k));
        u32 value = q << k | rem;

        return rice_to_signed(value);
    }

    /**
     * @param input 
     * @return ErrorOr<u64> 
     */
    ErrorOr<u64> read_utf8_char(BigEndianInputBitStream& input)
    {
        u64 character;
        u8 buffer = 0;
        Bytes buffer_bytes { &buffer, 1 };
        TRY(input.read(buffer_bytes));
        u8 start_byte = buffer_bytes[0];
       
        if ((start_byte & 0b10000000) == 0) {
            return start_byte;
        } else if ((start_byte & 0b11000000) == 0b10000000) {
            return Error::from_string_literal("Illegal continuation byte"sv);
        }
       
        u8 length = 1;

        while (((start_byte << length) & 0b10000000) == 0b10000000)
            ++length;

        u8 bits_from_start_byte = 8 - (length + 1);
        u8 start_byte_bitmask = Mods::exp2(bits_from_start_byte) - 1;
        character = start_byte_bitmask & start_byte;

        for (u8 i = length - 1; i > 0; --i) {
            TRY(input.read(buffer_bytes));
            u8 current_byte = buffer_bytes[0];
            character = (character << 6) | (current_byte & 0b00111111);
        }
        return character;
    }

    /**
     * @param n 
     * @param size 
     * @return i64 
     */
    i64 sign_extend(u32 n, u8 size)
    {
        if ((n & (1 << (size - 1))) > 0) {
            return static_cast<i64>(n | (0xffffffff << size));
        }

        return n;
    }

    /**
     * @param x 
     * @return i32 
     */
    i32 rice_to_signed(u32 x)
    {
        i32 sign = -static_cast<i32>(x & 1);
        return static_cast<i32>(sign ^ (x >> 1));
    }
}

