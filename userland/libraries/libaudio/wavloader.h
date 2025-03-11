/**
 * @file wavloader.h
 * @author Krisna Pranav
 * @brief wav loader
 * @version 6.0
 * @date 2025-02-21
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/memorystream.h>
#include <mods/ownptr.h>
#include <mods/refptr.h>
#include <mods/span.h>
#include <mods/stream.h>
#include <mods/string.h>
#include <mods/stringview.h>
#include <mods/weakptr.h>
#include <libaudio/buffer.h>
#include <libaudio/loader.h>
#include <libcore/file.h>
#include <libcore/filestream.h>

namespace Audio 
{

    #define WAVE_FORMAT_PCM 0x0001        
    #define WAVE_FORMAT_IEEE_FLOAT 0x0003 
    #define WAVE_FORMAT_ALAW 0x0006       
    #define WAVE_FORMAT_MULAW 0x0007      
    #define WAVE_FORMAT_EXTENSIBLE 0xFFFE 

    class WavLoaderPlugin : public LoaderPlugin 
    {
    public:
        /**
         * @brief Construct a new WavLoaderPlugin object
         * 
         * @param path 
         */
        explicit WavLoaderPlugin(StringView path);

        /**
         * @brief Construct a new WavLoaderPlugin object
         * 
         * @param buffer 
         */
        explicit WavLoaderPlugin(Bytes const& buffer);

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
        virtual MaybeLoaderError reset() override 
        { 
            return seek(0); 
        }

        /**
         * @param sample_index 
         * @return MaybeLoaderError 
         */
        virtual MaybeLoaderError seek(int const sample_index) override;

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
         * @return String 
         */
        virtual String format_name() override 
        { 
            return "RIFF WAVE (.wav)"; 
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

    private:
        MaybeLoaderError parse_header();

        RefPtr<Core::File> m_file;
        OwnPtr<Mods::InputStream> m_stream;

        Mods::InputMemoryStream* m_memory_stream;

        Optional<LoaderError> m_error {};

        u32 m_sample_rate { 0 };
        u16 m_num_channels { 0 };

        PcmSampleFormat m_sample_format;

        size_t m_byte_offset_of_data_samples { 0 };

        int m_loaded_samples { 0 };
        int m_total_samples { 0 };
    }; // class WavLoaderPlugin : public LoaderPlugin

} // namespace Audio
