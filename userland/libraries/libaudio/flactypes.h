/**
 * @file flactypes.h
 * @author Krisna Pranav
 * @brief flac types
 * @version 6.0
 * @date 2025-02-19
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include "buffer.h"
#include <mods/bytebuffer.h>
#include <mods/types.h>
#include <mods/variant.h>

namespace Audio
{

    #define FLAC_BLOCKSIZE_AT_END_OF_HEADER_8 0xffffffff
    #define FLAC_BLOCKSIZE_AT_END_OF_HEADER_16 0xfffffffe
    #define FLAC_SAMPLERATE_AT_END_OF_HEADER_8 0xffffffff
    #define FLAC_SAMPLERATE_AT_END_OF_HEADER_16 0xfffffffe
    #define FLAC_SAMPLERATE_AT_END_OF_HEADER_16X10 0xfffffffd

    enum class FlacMetadataBlockType : u8
    {
        STREAMINFO = 0,     
        PADDING = 1,        
        APPLICATION = 2,    
        SEEKTABLE = 3,      
        VORBIS_COMMENT = 4, 
        CUESHEET = 5,       
        PICTURE = 6,        
        INVALID = 127,      
    }; // enum class FlacMetadataBlockType : u8

    enum class FlacFrameChannelType : u8
    {
        Mono = 0,
        Stereo = 1,
        StereoCenter = 2,    
        Surround4p0 = 3,     
        Surround5p0 = 4,     
        Surround5p1 = 5,     
        Surround6p1 = 6,     
        Surround7p1 = 7,     
        LeftSideStereo = 8,  
        RightSideStereo = 9, 
        MidSideStereo = 10,  
    }; // enum class FlacFrameChannelType : u8

    enum class FlacSubframeType : u8
    {
        Constant = 0,
        Verbatim = 1,
        Fixed = 0b001000,
        LPC = 0b100000,
    }; // enum class FlacSubframeType : u8

    enum class FlacResidualMode : u8
    {
        Rice4Bit = 0,
        Rice5Bit = 1,
    }; // enum class FlacResidualMode : u8

    struct FlacRawMetadataBlock
    {
        bool is_last_block;
        FlacMetadataBlockType type;
        u32 length; 
        ByteBuffer data;
    }; // struct FlacRawMetadataBlock

    struct FlacFrameHeader
    {
        u32 sample_count;
        u32 sample_rate;
        FlacFrameChannelType channels;
        PcmSampleFormat bit_depth;
    }; // struct FlacFrameHeader

    struct FlacSubframeHeader
    {
        FlacSubframeType type;
        u8 order;
        u8 wasted_bits_per_sample;
        u8 bits_per_sample;
    }; // struct FlacSubframeHeader

    struct FlacSeekPoint
    {
        u64 sample_index;
        u64 byte_offset;
        u16 num_samples;
    }; // struct FlacSeekPoint

} // namespace Audio
