/**
 * @file wavwriter.cpp
 * @author Krisna Pranav
 * @brief wav writer
 * @version 6.0
 * @date 2025-02-18
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <libaudio/wavwriter.h>

namespace Audio
{
    /**
     * @brief Construct a new WavWriter::WavWriter object
     * 
     * @param path 
     * @param sample_rate 
     * @param num_channels 
     */
    WavWriter::WavWriter(StringView path, int sample_rate, u16 num_channels)
        : m_sample_rate(sample_rate)
        , m_num_channels(num_channels)
    {
        set_file(path);
    }

    /**
     * @brief Destroy the WavWriter::WavWriter object
     * 
     */
    WavWriter::~WavWriter()
    {
        if (!m_finalized)
            finalize();
    }

    /**
     * @param samples 
     * @param size 
     */
    void WavWriter::write_samples(u8 const* samples, size_t size)
    {
        m_data_sz += size;
        m_file->write(samples, size);
    }

    void WavWriter::finalize()
    {
        VERIFY(!m_finalized);
        m_finalized = true;

        if (m_file) {
            m_file->seek(0);
            write_header();
            m_file->close();
        }

        m_data_sz = 0;
    }

    void WavWriter::write_header()
    {
        
    }
} // namespace Audio