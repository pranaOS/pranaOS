/**
 * @file wavloader.cpp
 * @author Krisna Pranav
 * @brief wavloader
 * @version 6.0
 * @date 2025-02-21
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include "wavloader.h"
#include "buffer.h"
#include <mods/debug.h>
#include <mods/fixedarray.h>
#include <mods/numericlimits.h>
#include <mods/ownptr.h>
#include <mods/try.h>
#include <libcore/file.h>
#include <libcore/filestream.h>

namespace Audio 
{

    static constexpr size_t maximum_wav_size = 1 * GiB; 

    /**
     * @brief Construct a new WavLoaderPlugin::WavLoaderPlugin object
     * 
     * @param path 
     */
    WavLoaderPlugin::WavLoaderPlugin(StringView path)
        : m_file(Core::File::construct(path))
    {
        if (!m_file->open(Core::OpenMode::ReadOnly)) {
            m_error = LoaderError { String::formatted("Can't open file: {}", m_file->error_string()) };
            return;
        }
        m_stream = make<Core::InputFileStream>(*m_file);
    }

    /**
     * @return MaybeLoaderError 
     */
    MaybeLoaderError WavLoaderPlugin::initialize()
    {
        if (m_error.has_value())
            return m_error.release_value();
        TRY(parse_header());
        return {};
    }

    /**
     * @brief Construct a new WavLoaderPlugin::WavLoaderPlugin object
     * 
     * @param buffer 
     */
    WavLoaderPlugin::WavLoaderPlugin(Bytes const& buffer)
    {
        m_stream = make<InputMemoryStream>(buffer);

        if (!m_stream) {
            m_error = LoaderError { String::formatted("Can't open memory stream") };
            return;
        }

        m_memory_stream = static_cast<InputMemoryStream*>(m_stream.ptr());
    }   

    /**
     * @param max_bytes_to_read_from_input 
     * @return LoaderSamples 
     */
    LoaderSamples WavLoaderPlugin::get_more_samples(size_t max_bytes_to_read_from_input)
    {
        if (!m_stream)
            return LoaderError { LoaderError::Category::Internal, static_cast<size_t>(m_loaded_samples), "No stream" };

        int remaining_samples = m_total_samples - m_loaded_samples;

        if (remaining_samples <= 0)
            return FixedArray<Sample> {};

        size_t bytes_per_sample
            = m_num_channels * pcm_bits_per_sample(m_sample_format) / 8;

        int max_samples_to_read = static_cast<int>(max_bytes_to_read_from_input) / bytes_per_sample;
        int samples_to_read = min(max_samples_to_read, remaining_samples);

        size_t bytes_to_read = samples_to_read * bytes_per_sample;

        dbgln_if(AWAVLOADER_DEBUG, "Read {} bytes WAV with num_channels {} sample rate {}, "
                                "bits per sample {}, sample format {}",
            bytes_to_read, m_num_channels, m_sample_rate,
            pcm_bits_per_sample(m_sample_format), sample_format_name(m_sample_format));

        auto sample_data_result = ByteBuffer::create_zeroed(bytes_to_read);

        if (sample_data_result.is_error())
            return LoaderError { LoaderError::Category::IO, static_cast<size_t>(m_loaded_samples), "Couldn't allocate sample buffer" };

        auto sample_data = sample_data_result.release_value();

        m_stream->read_or_error(sample_data.bytes());

        if (m_stream->handle_any_error())
            return LoaderError { LoaderError::Category::IO, static_cast<size_t>(m_loaded_samples), "Stream read error" };

        auto buffer = LegacyBuffer::from_pcm_data(
            sample_data.bytes(),
            m_num_channels,
            m_sample_format);

        if (buffer.is_error())
            return LoaderError { LoaderError::Category::Internal, static_cast<size_t>(m_loaded_samples), "Couldn't allocate sample buffer" };

        m_loaded_samples += samples_to_read;
        return LOADER_TRY(buffer.value()->to_sample_array());
    }

    /**
     * @param sample_index 
     * @return MaybeLoaderError 
     */
    MaybeLoaderError WavLoaderPlugin::seek(int const sample_index)
    {
        dbgln_if(AWAVLOADER_DEBUG, "seek sample_index {}", sample_index);

        if (sample_index < 0 || sample_index >= m_total_samples)
            return LoaderError { LoaderError::Category::Internal, static_cast<size_t>(m_loaded_samples), "Seek outside the sample range" };

        size_t sample_offset = m_byte_offset_of_data_samples + (sample_index * m_num_channels * (pcm_bits_per_sample(m_sample_format) / 8));

        if (m_file) {
            m_file->seek(sample_offset);
        } else {
            m_memory_stream->seek(sample_offset);
        }

        m_loaded_samples = sample_index;
        return {};
    }

    /**
     * @return MaybeLoaderError 
     */
    MaybeLoaderError WavLoaderPlugin::parse_header()
    {
        if (!m_stream)
            return LoaderError { LoaderError::Category::Internal, 0, "No stream" };

        bool ok = true;
        size_t bytes_read = 0;

        auto read_u8 = [&]() -> u8 {
            u8 value;
            *m_stream >> value;
            if (m_stream->handle_any_error())
                ok = false;
            bytes_read += 1;
            return value;
        };

        auto read_u16 = [&]() -> u16 {
            u16 value;
            *m_stream >> value;
            if (m_stream->handle_any_error())
                ok = false;
            bytes_read += 2;
            return value;
        };

        auto read_u32 = [&]() -> u32 {
            u32 value;
            *m_stream >> value;
            if (m_stream->handle_any_error())
                ok = false;
            bytes_read += 4;
            return value;
        };

    #define CHECK_OK(category, msg)                                                            \
        do {                                                                                   \
            if (!ok)                                                                           \
                return LoaderError { category, String::formatted("Parsing failed: {}", msg) }; \
        } while (0)

        u32 riff = read_u32();

        ok = ok && riff == 0x46464952; 
        CHECK_OK(LoaderError::Category::Format, "RIFF header");

        u32 sz = read_u32();

        ok = ok && sz < maximum_wav_size;
        CHECK_OK(LoaderError::Category::Format, "File size");

        u32 wave = read_u32();

        ok = ok && wave == 0x45564157; 
        CHECK_OK(LoaderError::Category::Format, "WAVE header");

        u32 fmt_id = read_u32();

        ok = ok && fmt_id == 0x20746D66; 
        CHECK_OK(LoaderError::Category::Format, "FMT header");

        u32 fmt_size = read_u32();

        ok = ok && (fmt_size == 16 || fmt_size == 18 || fmt_size == 40);
        CHECK_OK(LoaderError::Category::Format, "FMT size");

        u16 audio_format = read_u16();

        CHECK_OK(LoaderError::Category::Format, "Audio format"); 
        ok = ok && (audio_format == WAVE_FORMAT_PCM || audio_format == WAVE_FORMAT_IEEE_FLOAT || audio_format == WAVE_FORMAT_EXTENSIBLE);
        CHECK_OK(LoaderError::Category::Unimplemented, "Audio format PCM/Float"); 

        m_num_channels = read_u16();
        ok = ok && (m_num_channels == 1 || m_num_channels == 2);
        CHECK_OK(LoaderError::Category::Unimplemented, "Channel count");

        m_sample_rate = read_u32();
        CHECK_OK(LoaderError::Category::IO, "Sample rate");

        read_u32();
        CHECK_OK(LoaderError::Category::IO, "Data rate");

        u16 block_size_bytes = read_u16();
        CHECK_OK(LoaderError::Category::IO, "Block size");

        u16 bits_per_sample = read_u16();
        CHECK_OK(LoaderError::Category::IO, "Bits per sample");

        if (audio_format == WAVE_FORMAT_EXTENSIBLE) {
            ok = ok && (fmt_size == 40);
            CHECK_OK(LoaderError::Category::Format, "Extensible fmt size"); 

            read_u32();
            read_u32();
            CHECK_OK(LoaderError::Category::IO, "Discard until GUID");

            u16 guid_subformat = read_u16();
            ok = ok && (guid_subformat == WAVE_FORMAT_PCM || guid_subformat == WAVE_FORMAT_IEEE_FLOAT);
            CHECK_OK(LoaderError::Category::Unimplemented, "GUID SubFormat");

            audio_format = guid_subformat;
        }

        if (audio_format == WAVE_FORMAT_PCM) {
            ok = ok && (bits_per_sample == 8 || bits_per_sample == 16 || bits_per_sample == 24);
            CHECK_OK(LoaderError::Category::Unimplemented, "Bits per sample (PCM)"); 

            if (bits_per_sample == 8) {
                m_sample_format = PcmSampleFormat::Uint8;
            } else if (bits_per_sample == 16) {
                m_sample_format = PcmSampleFormat::Int16;
            } else if (bits_per_sample == 24) {
                m_sample_format = PcmSampleFormat::Int24;
            }
        } else if (audio_format == WAVE_FORMAT_IEEE_FLOAT) {
            ok = ok && (bits_per_sample == 32 || bits_per_sample == 64);
            CHECK_OK(LoaderError::Category::Unimplemented, "Bits per sample (Float)"); 

            if (bits_per_sample == 32) {
                m_sample_format = PcmSampleFormat::Float32;
            } else if (bits_per_sample == 64) {
                m_sample_format = PcmSampleFormat::Float64;
            }
        }

        ok = ok && (block_size_bytes == (m_num_channels * (bits_per_sample / 8)));
        CHECK_OK(LoaderError::Category::Format, "Block size sanity check");

        dbgln_if(AWAVLOADER_DEBUG, "WAV format {} at {} bit, {} channels, rate {}Hz ",
            sample_format_name(m_sample_format), pcm_bits_per_sample(m_sample_format), m_num_channels, m_sample_rate);

        bool found_data = false;
        u32 data_sz = 0;
        u8 search_byte = 0;
        while (true) {
            search_byte = read_u8();
            CHECK_OK(LoaderError::Category::IO, "Reading byte searching for data");
            if (search_byte != 0x64) 
                continue;

            search_byte = read_u8();
            CHECK_OK(LoaderError::Category::IO, "Reading next byte searching for data");
            if (search_byte != 0x61) 
                continue;

            u16 search_remaining = read_u16();
            CHECK_OK(LoaderError::Category::IO, "Reading remaining bytes searching for data");
            if (search_remaining != 0x6174) 
                continue;

            data_sz = read_u32();
            found_data = true;
            break;
        }

        ok = ok && found_data;
        CHECK_OK(LoaderError::Category::Format, "Found no data chunk");

        ok = ok && data_sz < maximum_wav_size;
        CHECK_OK(LoaderError::Category::Format, "Data was too large");

        m_total_samples = data_sz / block_size_bytes;

        dbgln_if(AWAVLOADER_DEBUG, "WAV data size {}, bytes per sample {}, total samples {}",
            data_sz,
            block_size_bytes,
            m_total_samples);

        m_byte_offset_of_data_samples = bytes_read;
        return {};
    }
}
