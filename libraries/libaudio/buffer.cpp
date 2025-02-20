/**
 * @file buffer.cpp
 * @author Krisna Pranav
 * @brief buffer
 * @version 6.0
 * @date 2025-02-20
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include "buffer.h"
#include <mods/atomic.h>
#include <mods/debug.h>
#include <mods/stdlibextras.h>
#include <mods/string.h>

namespace Audio 
{
    /**
     * @return i32 
     */
    i32 LegacyBuffer::allocate_id()
    {
        static Atomic<i32> next_id;
        return next_id++;
    }

    /**
     * @tparam SampleReader 
     * @param stream 
     * @param read_sample 
     * @param samples 
     * @param num_channels 
     */
    template<typename SampleReader>
    static void read_samples_from_stream(InputMemoryStream& stream, SampleReader read_sample, Vector<Sample>& samples, int num_channels)
    {
        double left_channel_sample = 0;
        double right_channel_sample = 0;

        switch (num_channels) {
        case 1:
            for (;;) {
                left_channel_sample = read_sample(stream);
                samples.append(Sample(left_channel_sample));

                if (stream.handle_any_error()) {
                    break;
                }
            }
            break;
        case 2:
            for (;;) {
                left_channel_sample = read_sample(stream);
                right_channel_sample = read_sample(stream);
                samples.append(Sample(left_channel_sample, right_channel_sample));

                if (stream.handle_any_error()) {
                    break;
                }
            }
            break;
        default:
            VERIFY_NOT_REACHED();
        }
    }

    /**
     * @param stream 
     * @return double 
     */
    static double read_float_sample_64(InputMemoryStream& stream)
    {
        LittleEndian<double> sample;
        stream >> sample;
        return double(sample);
    }

    /**
     * @param stream 
     * @return double 
     */
    static double read_float_sample_32(InputMemoryStream& stream)
    {
        LittleEndian<float> sample;
        stream >> sample;
        return double(sample);
    }

    /**
     * @param stream 
     * @return double 
     */
    static double read_norm_sample_24(InputMemoryStream& stream)
    {
        u8 byte = 0;
        stream >> byte;
        u32 sample1 = byte;
        stream >> byte;
        u32 sample2 = byte;
        stream >> byte;
        u32 sample3 = byte;

        i32 value = 0;
        value = sample1 << 8;
        value |= (sample2 << 16);
        value |= (sample3 << 24);
        return double(value) / NumericLimits<i32>::max();
    }

    /**
     * @param stream 
     * @return double 
     */
    static double read_norm_sample_16(InputMemoryStream& stream)
    {
        LittleEndian<i16> sample;
        stream >> sample;
        return double(sample) / NumericLimits<i16>::max();
    }   

    /**
     * @param stream 
     * @return double 
     */
    static double read_norm_sample_8(InputMemoryStream& stream)
    {
        u8 sample = 0;
        stream >> sample;
        return double(sample) / NumericLimits<u8>::max();
    }

    /**
     * @param data 
     * @param num_channels 
     * @param sample_format 
     * @return ErrorOr<NonnullRefPtr<LegacyBuffer>> 
     */
    ErrorOr<NonnullRefPtr<LegacyBuffer>> LegacyBuffer::from_pcm_data(ReadonlyBytes data, int num_channels, PcmSampleFormat sample_format)
    {
        InputMemoryStream stream { data };
        return from_pcm_stream(stream, num_channels, sample_format, data.size() / (pcm_bits_per_sample(sample_format) / 8));
    }

    /**
     * @param stream 
     * @param num_channels 
     * @param sample_format 
     * @param num_samples 
     * @return ErrorOr<NonnullRefPtr<LegacyBuffer>> 
     */
    ErrorOr<NonnullRefPtr<LegacyBuffer>> LegacyBuffer::from_pcm_stream(InputMemoryStream& stream, int num_channels, PcmSampleFormat sample_format, int num_samples)
    {
        Vector<Sample> fdata;
        fdata.ensure_capacity(num_samples);

        switch (sample_format) {
        case PcmSampleFormat::Uint8:
            read_samples_from_stream(stream, read_norm_sample_8, fdata, num_channels);
            break;
        case PcmSampleFormat::Int16:
            read_samples_from_stream(stream, read_norm_sample_16, fdata, num_channels);
            break;
        case PcmSampleFormat::Int24:
            read_samples_from_stream(stream, read_norm_sample_24, fdata, num_channels);
            break;
        case PcmSampleFormat::Float32:
            read_samples_from_stream(stream, read_float_sample_32, fdata, num_channels);
            break;
        case PcmSampleFormat::Float64:
            read_samples_from_stream(stream, read_float_sample_64, fdata, num_channels);
            break;
        default:
            VERIFY_NOT_REACHED();
        }

        VERIFY(!stream.handle_any_error());

        return LegacyBuffer::create_with_samples(move(fdata));
    } 

} // namespace Audio
