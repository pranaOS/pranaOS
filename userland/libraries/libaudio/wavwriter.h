/**
 * @file wavwriter.h
 * @author Krisna Pranav
 * @brief WAV Writer
 * @version 6.0
 * @date 2025-02-18
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <libcore/file.h>
#include <mods/noncopyable.h>
#include <mods/stringview.h>

namespace Audio
{

    class WavWriter
    {
        MOD_MAKE_NONCOPYABLE(WavWriter);
        MOD_MAKE_NONMOVABLE(WavWriter);

    public:
        /**
         * @brief Construct a new WavWriter object
         * 
         * @param path 
         * @param sample_rate 
         * @param num_channels 
         * @param bits_per_sample 
         */
        WavWriter(StringView path, int sample_rate = 44100, u16 num_channels = 2, u16 bits_per_sample = 16);

        /**
         * @brief Construct a new WavWriter object
         * 
         * @param sample_rate 
         * @param num_channels 
         * @param bits_per_sample 
         */
        WavWriter(int sample_rate = 44100, u16 num_channels = 2, u16 bits_per_sample = 16);
        ~WavWriter();

        /**
         * @return true 
         * @return false 
         */
        bool has_error() const
        {
            return !m_error_string.is_null();
        }

        /**
         * @return char const* 
         */
        char const* error_string() const
        {
            return m_error_string.characters();
        }

        /**
         * @param samples 
         * @param size 
         */
        void write_samples(u8 const* samples, size_t size);
        
        void finalize(); 

        /**
         * @return u32 
         */
        u32 sample_rate() const
        {
            return m_sample_rate;
        }
        
        /**
         * @return u16 
         */
        u16 num_channels() const
        {
            return m_num_channels;
        }

        /**
         * @return u16 
         */
        u16 bits_per_sample() const
        {
            return m_bits_per_sample;
        }

        /**
         * @return RefPtr<Core::File> 
         */
        RefPtr<Core::File> file() const
        {
            return m_file;
        }

        /**
         * @brief Set the file object
         * 
         * @param path 
         */
        void set_file(StringView path);

        /**
         * @brief Set the num channels object
         * 
         * @param num_channels 
         */
        void set_num_channels(int num_channels)
        {
            m_num_channels = num_channels;
        }

        /**
         * @brief Set the sample rate object
         * 
         * @param sample_rate 
         */
        void set_sample_rate(int sample_rate)
        {
            m_sample_rate = sample_rate;
        }
        
        /**
         * @brief Set the bits per sample object
         * 
         * @param bits_per_sample 
         */
        void set_bits_per_sample(int bits_per_sample)
        {
            m_bits_per_sample = bits_per_sample;
        }

        void clear_error()
        {
            m_error_string = String();
        }

    private:
        void write_header();
        RefPtr<Core::File> m_file;
        String m_error_string;
        bool m_finalized{false};

        u32 m_sample_rate;
        u16 m_num_channels;
        u16 m_bits_per_sample;
        u32 m_data_sz{0};
    }; // class WavWriter

} // namespace Audio
