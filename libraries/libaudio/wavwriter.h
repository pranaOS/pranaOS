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