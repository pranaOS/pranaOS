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

    /**
     * @param channel_type 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE u8 frame_channel_type_to_channel_count(FlacFrameChannelType channel_type);

    /**
     * @param n 
     * @param size 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE i64 sign_extend(u32 n, u8 size);
    
    /**
     * @param x 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE i32 rice_to_signed(u32 x);

    /**
     * @param input 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE ErrorOr<u64> read_utf8_char(BigEndianInputBitStream& input);
    
    /**
     * @param order 
     * @param bit_input 
     * @return ALWAYS_INLINE 
     */
    ALWAYS_INLINE ErrorOr<i32> decode_unsigned_exp_golomb(u8 order, BigEndianInputBitStream& bit_input);

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
        {
        }   

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
        virtual MaybeLoaderError seek(int sample_index) override;

        /**
         * @return int 
         */
        virtual int loaded_samples() override 
        { 
            return static_cast<int>(m_loaded_samples); 
        }

        /**
         * @return int 
         */
        virtual int total_samples() override 
        { 
            return static_cast<int>(m_total_samples); 
        }

        /**
         * @return u32 
         */
        virtual u32 sample_rate() override 
        { 
            return m_sample_rate; 
        }
        
        /**
         * @return u16 
         */
        virtual u16 num_channels() override 
        { 
            return m_num_channels; 
        }

        /**
         * @return String 
         */
        virtual String format_name() override 
        { 
            return "FLAC (.flac)"; 
        }

        /**
         * @return PcmSampleFormat 
         */
        virtual PcmSampleFormat pcm_format() override 
        { 
            return m_sample_format; 
        }

        /**
         * @return RefPtr<Core::File> 
         */
        virtual RefPtr<Core::File> file() override 
        { 
            return m_file; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_fixed_blocksize_stream() const 
        { 
            return m_min_block_size == m_max_block_size; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool sample_count_unknown() const 
        { 
            return m_total_samples == 0; 
        }

    private:
        /**
         * @return MaybeLoaderError 
         */
        MaybeLoaderError parse_header();
        
        /**
         * @param bit_input 
         * @return ErrorOr<FlacRawMetadataBlock, LoaderError> 
         */
        ErrorOr<FlacRawMetadataBlock, LoaderError> next_meta_block(BigEndianInputBitStream& bit_input);
        
        /**
         * @return MaybeLoaderError 
         */
        MaybeLoaderError next_frame(Span<Sample>);
        
        /**
         * @param bit_input 
         * @param channel_index 
         * @return ErrorOr<FlacSubframeHeader, LoaderError> 
         */
        ErrorOr<FlacSubframeHeader, LoaderError> next_subframe_header(BigEndianInputBitStream& bit_input, u8 channel_index);
        
        /**
         * @param subframe_header 
         * @param bit_input 
         * @return ErrorOr<Vector<i32>, LoaderError> 
         */
        ErrorOr<Vector<i32>, LoaderError> parse_subframe(FlacSubframeHeader& subframe_header, BigEndianInputBitStream& bit_input);
        
        /**
         * @param subframe 
         * @param bit_input 
         * @return ErrorOr<Vector<i32>, LoaderError> 
         */
        ErrorOr<Vector<i32>, LoaderError> decode_fixed_lpc(FlacSubframeHeader& subframe, BigEndianInputBitStream& bit_input);

        /**
         * @param subframe 
         * @param bit_input 
         * @return ErrorOr<Vector<i32>, LoaderError> 
         */
        ErrorOr<Vector<i32>, LoaderError> decode_verbatim(FlacSubframeHeader& subframe, BigEndianInputBitStream& bit_input);

        /**
         * @param subframe 
         * @param bit_input 
         * @return ErrorOr<Vector<i32>, LoaderError> 
         */
        ErrorOr<Vector<i32>, LoaderError> decode_custom_lpc(FlacSubframeHeader& subframe, BigEndianInputBitStream& bit_input);

        /**
         * @param decoded 
         * @param subframe 
         * @param bit_input 
         * @return MaybeLoaderError 
         */
        MaybeLoaderError decode_residual(Vector<i32>& decoded, FlacSubframeHeader& subframe, BigEndianInputBitStream& bit_input);
        
        /**
         * @param partition_type 
         * @param partitions 
         * @param partition_index 
         * @param subframe 
         * @param bit_input 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE ErrorOr<Vector<i32>, LoaderError> decode_rice_partition(u8 partition_type, u32 partitions, u32 partition_index, FlacSubframeHeader& subframe, BigEndianInputBitStream& bit_input);

        /**
         * @return MaybeLoaderError 
         */
        MaybeLoaderError load_seektable(FlacRawMetadataBlock&);

        /**
         * @param sample_count_code 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE ErrorOr<u32, LoaderError> convert_sample_count_code(u8 sample_count_code);

        /**
         * @param sample_rate_code 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE ErrorOr<u32, LoaderError> convert_sample_rate_code(u8 sample_rate_code);

        /**
         * @param bit_depth_code 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE ErrorOr<PcmSampleFormat, LoaderError> convert_bit_depth_code(u8 bit_depth_code);

        RefPtr<Core::File> m_file;
        Optional<LoaderError> m_error {};

        u32 m_sample_rate { 0 };         
        u8 m_num_channels { 0 };         

        PcmSampleFormat m_sample_format; 
        
        u16 m_min_block_size { 0 };
        u16 m_max_block_size { 0 };
        
        u32 m_min_frame_size { 0 }; 
        u32 m_max_frame_size { 0 }; 
        u64 m_total_samples { 0 };  
        u8 m_md5_checksum[128 / 8]; 

        size_t m_loaded_samples { 0 };

        u64 m_data_start_location { 0 };
        OwnPtr<Core::Stream::SeekableStream> m_stream;
        Optional<FlacFrameHeader> m_current_frame;

        Vector<Sample, FLAC_BUFFER_SIZE> m_unread_data;
        u64 m_current_sample_or_frame { 0 };
        Vector<FlacSeekPoint> m_seektable;
    }; // class FlacLoaderPlugin : public LoaderPlugin

} // namespace Audio
