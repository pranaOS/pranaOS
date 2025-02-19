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
     * @return String 
     */
    String sample_format_name(PcmSampleFormat format)
    {
        bool is_float = format == PcmSampleFormat::Float32;
        return String::formatted("", pcm_bits_per_sample(format));
    }
} // namespace Audio