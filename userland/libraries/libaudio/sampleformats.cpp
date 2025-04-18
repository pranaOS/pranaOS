/**
 * @file sampleformats.cpp
 * @author Krisna Pranav
 * @brief Sample Formats
 * @version 6.0
 * @date 2025-02-19
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#include "sampleformats.h"

namespace Audio
{
    /**
     * @param format 
     * @return u16 
     */
    u16 pcm_bits_per_sample(PcmSampleFormat format)
    {
        switch(format)
        {
        case PcmSampleFormat::Uint8:
            return 8;
        case PcmSampleFormat::Int16:
            return 16;
        case PcmSampleFormat::Int24:
            return 24;
        case PcmSampleFormat::Int32:
        case PcmSampleFormat::Float32:
            return 32;
        case PcmSampleFormat::Float64:
            return 64;
        default:
            VERIFY_NOT_REACHED();
        }
    }

    /**
     * @param format 
     * @return String 
     */
    String sample_format_name(PcmSampleFormat format)
    {
        bool is_float = format == PcmSampleFormat::Float32 || format == PcmSampleFormat::Float64;
        return String::formatted("PCM {}bit {}", pcm_bits_per_sample(format), is_float ? "Float" : "LE");
    }

} // namespace Audio