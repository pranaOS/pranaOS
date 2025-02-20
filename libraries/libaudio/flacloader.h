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
    public:

        /**
         * @brief Construct a new FlacLoaderPlugin object
         * 
         * @param path 
         */
        explicit FlacLoaderPlugin(StringView path);

        /**
         * @brief Construct a new FlacLoaderPlugin object
         * 
         * @param buffer 
         */
        explicit FlacLoaderPlugin(Bytes& buffer);

        /**
         * @brief Destroy the FlacLoaderPlugin object
         * 
         */
        ~FlacLoaderPlugin()
        {}

        /**
         * @return MaybeLoaderError 
         */
        virtual MaybeLoaderError initialize() override;

        /**
         * @brief Get the more samples object
         * 
         * @param max_bytes_to_read_from_input 
         * @return LoaderSamples 
         */
        virtual LoaderSamples get_more_samples(size_t max_bytes_to_read_from_input = 128 * KiB) override;

        /**
         * @return MaybeLoaderError 
         */
        virtual MaybeLoaderError reset() override;

        /**
         * @param sample_index 
         * @return MaybeLoaderError 
         */
        virtual MaybeLoaderError seek(int sample_index) override

        /**
         * @return true 
         * @return false 
         */
        bool is_fixed_blocksize_stream() const
        {
            return m_min_block_size;
        }

        /**
         * @return true 
         * @return false 
         */
        bool sample_count_unknown() const
        {
            return m_total_samples;
        }

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

        u16 m_min_block_size { 0 };
        u16 m_max_block_size { 0 };
    }; // class FlacLoaderPlugin : public LoaderPlugin
} // namespace Audio