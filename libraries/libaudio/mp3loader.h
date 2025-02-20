/**
 * @file mp3loader.h
 * @author Krisna Pranav
 * @brief mp3 loader
 * @version 6.0
 * @date 2025-02-20
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "buffer.h"
#include "loader.h"
#include "mp3types.h"
#include <mods/tuple.h>
#include <libcore/filestream.h>
#include <libdsp/mdct.h>

namespace Audio {

    namespace MP3::Tables 
    {
        struct ScaleFactorBand;
    } // namespace MP3::Tables

    class MP3LoaderPlugin : public LoaderPlugin 
    {
    public:
        /**
         * @brief Construct a new MP3LoaderPlugin object
         * 
         * @param path 
         */
        explicit MP3LoaderPlugin(StringView path);

        /**
         * @brief Destroy the MP3LoaderPlugin object
         * 
         */
        ~MP3LoaderPlugin();

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
         * @param position 
         * @return MaybeLoaderError 
         */
        virtual MaybeLoaderError seek(int const position) override;

        /**
         * @return int 
         */
        virtual int loaded_samples() override 
        { 
            return m_loaded_samples; 
        }
        
        /**
         * @return int 
         */
        virtual int total_samples() override 
        { 
            return m_total_samples; 
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
         * @return String 
         */
        virtual String format_name() override 
        { 
            return "MP3 (.mp3)"; 
        }

    private:
        /**
         * @return MaybeLoaderError 
         */
        MaybeLoaderError synchronize();

        /**
         * @return MaybeLoaderError 
         */
        MaybeLoaderError build_seek_table();

        /**
         * @return ErrorOr<MP3::Header, LoaderError> 
         */
        ErrorOr<MP3::Header, LoaderError> read_header();

        /**
         * @return ErrorOr<MP3::MP3Frame, LoaderError> 
         */
        ErrorOr<MP3::MP3Frame, LoaderError> read_next_frame();

        /**
         * @param is_first_frame 
         * @return ErrorOr<MP3::MP3Frame, LoaderError> 
         */
        ErrorOr<MP3::MP3Frame, LoaderError> read_frame_data(MP3::Header const&, bool is_first_frame);

        /**
         * @return MaybeLoaderError 
         */
        MaybeLoaderError read_side_information(MP3::MP3Frame&);

        /**
         * @param reservoir 
         * @param granule_index 
         * @param channel_index 
         * @return ErrorOr<size_t, LoaderError> 
         */
        ErrorOr<size_t, LoaderError> read_scale_factors(MP3::MP3Frame&, InputBitStream& reservoir, size_t granule_index, size_t channel_index);

        /**
         * @param reservoir 
         * @param granule_index 
         * @param channel_index 
         * @param granule_bits_read 
         * @return MaybeLoaderError 
         */
        MaybeLoaderError read_huffman_data(MP3::MP3Frame&, InputBitStream& reservoir, size_t granule_index, size_t channel_index, size_t granule_bits_read);

        /**
         * @param granule_index 
         * @param channel_index 
         * @return Mods::Array<double, 576> 
         */
        static Mods::Array<double, 576> calculate_frame_exponents(MP3::MP3Frame const&, size_t granule_index, size_t channel_index);

        /**
         * @param sample_rate 
         */
        static void reorder_samples(MP3::Granule&, u32 sample_rate);

        /**
         * @param max_subband_index 
         */
        static void reduce_alias(MP3::Granule&, size_t max_subband_index = 576);

        /**
         * @param granule_index 
         */
        static void process_stereo(MP3::MP3Frame&, size_t granule_index);

        /**
         * @param input 
         * @param input_offset 
         * @param output 
         * @param block_type 
         */
        static void transform_samples_to_time(Array<double, 576> const& input, size_t input_offset, Array<double, 36>& output, MP3::BlockType block_type);

        /**
         * @param V 
         * @param samples 
         * @param result 
         */
        static void synthesis(Array<double, 1024>& V, Array<double, 32>& samples, Array<double, 32>& result);

        /**
         * @brief Get the scalefactor bands object
         * 
         * @param samplerate 
         * @return Span<MP3::Tables::ScaleFactorBand const> 
         */
        static Span<MP3::Tables::ScaleFactorBand const> get_scalefactor_bands(MP3::Granule const&, int samplerate);


        Mods::Vector<Mods::Tuple<size_t, int>> m_seek_table;
        Mods::Array<Mods::Array<Mods::Array<double, 18>, 32>, 2> m_last_values {};
        Mods::Array<Mods::Array<double, 1024>, 2> m_synthesis_buffer {};

        static LibDSP::MDCT<36> s_mdct_36;
        static LibDSP::MDCT<12> s_mdct_12;

        u32 m_sample_rate { 0 };
        u8 m_num_channels { 0 };

        PcmSampleFormat m_sample_format { PcmSampleFormat::Int16 };

        size_t m_file_size { 0 };
        int m_total_samples { 0 };
        size_t m_loaded_samples { 0 };
        bool m_is_first_frame { true };

        Mods::Optional<MP3::MP3Frame> m_current_frame;
        u32 m_current_frame_read;
        RefPtr<Core::File> m_file;
        OwnPtr<Core::InputFileStream> m_input_stream;
        OwnPtr<InputBitStream> m_bitstream;
        DuplexMemoryStream m_bit_reservoir;
        Optional<LoaderError> m_error {};
    }; // class MP3LoaderPlugin : public LoaderPlugin 

} // namespace Audio
