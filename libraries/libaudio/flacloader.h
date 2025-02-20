/**
 * @file flacloader.h
 * @author Krisna Pranav
 * @brief flac loader
 * @version 6.0
 * @date 2025-02-20
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include "buffer.h"
#include "flactypes.h"
#include "loader.h"
#include <mods/error.h>
#include <mods/span.h>
#include <mods/types.h>
#include <libcore/inputbitstream.h>
#include <libcore/memorystream.h>
#include <libcore/stream.h>

namespace Audio
{
    using Core::Stream::BigEndianInputBitStream;
    
    constexpr size_t FLAC_BUFFER_SIZE = 8 * KiB;

    class FlacLoaderPlugin : public LoaderPlugin
    {
    private:
        MaybeLoaderError parse_header();

        /**
         * @param bitstream 
         * @return ErrorOr<FlacRawMetadataBlock, LoaderError> 
         */
        ErrorOr<FlacRawMetadataBlock, LoaderError> next_meta_block(BigEndianInputBitStream& bitstream);

        /**
         * @return MaybeLoaderError 
         */
        MaybeLoaderError next_frame(Span<Sample>);

        /**
         * @return ErrorOr<FlacSubframeHeader, LoaderError> 
         */
        ErrorOr<FlacSubframeHeader, LoaderError> next_subframe_header();

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE ErrorOr<Vector<i32>, LoaderError> decode_rice_partition();
    }; // class FlacLoaderPlugin : public LoaderPlugin
} // namespace Audio