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

    #define WAVE_FORMAT_PCM 0x0001        // PCM
    #define WAVE_FORMAT_IEEE_FLOAT 0x0003 // IEEE float
    #define WAVE_FORMAT_ALAW 0x0006       // 8-bit ITU-T G.711 A-law
    #define WAVE_FORMAT_MULAW 0x0007      // 8-bit ITU-T G.711 µ-law
    #define WAVE_FORMAT_EXTENSIBLE 0xFFFE // Determined by SubFormat

    // Parses a WAV file and produces an Audio::LegacyBuffer.
    class WavLoaderPlugin : public LoaderPlugin {
    public:
        explicit WavLoaderPlugin(StringView path);
        explicit WavLoaderPlugin(Bytes const& buffer);

        virtual MaybeLoaderError initialize() override;

        // The Buffer returned contains input data resampled at the
        // destination audio device sample rate.
        virtual LoaderSamples get_more_samples(size_t max_bytes_to_read_from_input = 128 * KiB) override;

        virtual MaybeLoaderError reset() override { return seek(0); }

        // sample_index 0 is the start of the raw audio sample data
        // within the file/stream.
        virtual MaybeLoaderError seek(int const sample_index) override;

        virtual int loaded_samples() override { return m_loaded_samples; }
        virtual int total_samples() override { return m_total_samples; }
        virtual u32 sample_rate() override { return m_sample_rate; }
        virtual u16 num_channels() override { return m_num_channels; }
        virtual String format_name() override { return "RIFF WAVE (.wav)"; }
        virtual PcmSampleFormat pcm_format() override { return m_sample_format; }
        virtual RefPtr<Core::File> file() override { return m_file; }

    private:
        MaybeLoaderError parse_header();

        RefPtr<Core::File> m_file;
        OwnPtr<AK::InputStream> m_stream;
        AK::InputMemoryStream* m_memory_stream;
        Optional<LoaderError> m_error {};

        u32 m_sample_rate { 0 };
        u16 m_num_channels { 0 };
        PcmSampleFormat m_sample_format;
        size_t m_byte_offset_of_data_samples { 0 };

        int m_loaded_samples { 0 };
        int m_total_samples { 0 };
    };

}
