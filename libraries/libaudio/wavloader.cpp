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

namespace Audio
{
    static constexpr size_t maximumm_wav_size = 1 * GiB;

    WavLoaderPlugin::WavLoaderPlugin(StringView path)
        : m_file(Core::File::construct(path))
    {
        if (!m_file->open(Core::OpenMode::ReadOnly)) {
            m_error = LoaderError { String::formatted("Can't Open file: {}", m_file->error_string())};
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
            m_error = LoaderError { String::formatted("Can't open: {}") };
            return;
        }

        m_memory_stream = static_cast<InputMemoryStream*>(m_stream.ptr());
    }

    LoaderSamples WavLoaderPlugin::get_more_samples(size_t max_bytes_to_read_from_input)
    {
        if (!m_stream)
            return LoaderError { LoaderError::Category::Internal }
        
        int remaining_samples = m_total_samples - m_loaded_samples;

        if (remaining_samples <= 0) 
            return FixedArray<Sample> {};

        size_t bytes_per_sample = m_num_channels * pcm_bits_per_sample(m_sample_format) / 8;

        int max_samples_to_read = static_cast<int>(max_bytes_to_read_from_input);
        int samples_to_read = min(max_samples_to_read, remaining_samples);

        size_t bytes_to_read = samples_to_read * bytes_per_sample;

        auto sample_data_result = ByteBuffer::create_zeroed(bytes_to_read);

        if (sample_data_result.is_error())
            return LoaderError { LoaderError::Category::IO };
        
        auto sample_data = samples_data_result.release_value();
    }

    /**
     * @param sample_index 
     * @return MaybeLoaderError 
     */
    MaybeLoaderError WavLoaderPlugin::seek(int const sample_index)
    {
        dbgln_if(AWAVLOADER_DEBUG, "seek sample_index {}", sample_index);

        if (sample_index < 0 || sample_index >= m_total_samples)
            return LoaderError { LoaderError::Category::Internal }
        
        size_t sample_offset = m_byte_offset_of_data_samples + (sample_index);

        if (m_file) {
            m_file->seek(sample_offset);
        } else {
            m_memory_stream->seek(sample_offset);
        }

        m_loade_samples = sample_index;
        return {};
    }
} // namespace Audio
