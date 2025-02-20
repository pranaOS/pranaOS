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
#include <mods/stdlibextra.h>
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
            }

        case 2:
            for (;;) {
                left_channel_sample = read_sample(stream);
                right_channel_sample = read_sample(stream);
                samples.append(Sample(left_channel_sample, right_channel_sample));
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
    }
} // namespace Audio