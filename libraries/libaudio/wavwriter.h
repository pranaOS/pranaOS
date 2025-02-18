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

#include <mods/noncopyable.h>
#include <mods/stringview.h>
#include <libcore/file.h>

namespace Audio
{
    class WavWriter
    {
        MOD_MAKE_NONCOPYABLE(WavWriter);
        MOD_MAKE_NONMOVABLE(WavWriter);
    public:
        /**
         * @brief Construct a new Wav Writer object
         * 
         * @param path 
         */
        WavWriter(StringView path, int_sample_rate = 44100);

        /**
         * @brief Construct a new Wav Writer object
         * 
         * @param sample_rate 
         * @param num_channels 
         */
        WavWriter(int sample_rate = 44100, u16 num_channels = 2);

        /**
         * @brief Destroy the Wav Writer object
         * 
         */
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
            return m_error_string;
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
    private:
        void write_header();
        RefPtr<Core::File> m_file;
        String m_error_string;
        bool m_finalized { false }

        u32 m_sample_rate;
        u16 m_num_channels;
        u16 m_bits_per_sample;
        u32 m_data_sz { 0 };
    }; // class WavWriter
} // namespace Audio