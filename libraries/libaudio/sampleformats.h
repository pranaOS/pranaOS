/**
 * @file sampleformats.h
 * @author Krisna Pranav
 * @brief Sample Formats
 * @version 6.0
 * @date 2025-02-19
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/string.h>
#include <mods/types.h>

namespace Audio
{

    enum class PcmSampleFormat : u8
    {
        Uint8,
        Int16,
        Int24,
        Int32,
        Float32,
        Float64,
    }; // enum class PcmSampleFormat : u8

    /**
     * @param format 
     * @return u16 
     */
    u16 pcm_bits_per_sample(PcmSampleFormat format);

    /**
     * @param format 
     * @return String 
     */
    String sample_format_name(PcmSampleFormat format);
    
} // namespace Audio