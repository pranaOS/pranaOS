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
    #define WAVE_FORMAT_PCM
    #define WAVE_FORMAT_IEEE_FLOAT
    #define WAVE_FORMAT_ALAW
    #define WAVE_FORMAT_MULAW
    #define WAVE_FORMAT_EXTENSIBLE

    class WavLoaderPlugin : public LoaderPlugin
    {
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