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
    #define FLAC_BLOCKSIZE_AT_END_OF_HEADER_8
    #define FLAC_BLOCKSIZE_AT_END_OF_HEADER_16
    #define FLAC_BLOCKSIZE_AT_END_OF_HEADER_8
    #define FLAC_BLOCKSIZE_AT_END_OF_HEADER_16
    #define FLAC_BLOCKSIZE_AT_END_OF_HEADER_16x10
    
    enum class FlacMetadataBlockType : u8 
    {
        STREAMINFO = 0,
        PADDING = 1,
        APPLICATION = 2,
        SEEKTABLE = 3,
        VORBIS_COMMENT = 4,
        CUESHEET = 5,
    }; // enum class FlacMetadataBlockType : u8 

    enum class FlacFrameChannelType : u8
    {
        
    }; // enum class FlacFrameChannelType : u8

    enum class FlacSubframeType : u8
    {

    }; // enum class FlacSubframeType : u8

    enum class FlacResidualMode : u8
    {

    }; // enum class FlacResidualMode : u8

    struct FlacRawMetadataBlock
    {

    }; // struct FlacRawMetadataBlock

    struct FlacFrameHeader
    {

    }; // struct FlacFrameHeader

    struct FlacSubframeHeader
    {

    }; // struct FlacSubframeHeader

    struct FlacSeekPoint
    {

    }; // struct FlacSeekPoint
} // namespace Audio