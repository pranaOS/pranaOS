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
     * @param bits_per_sample 
     */
    WavWriter::WavWriter(StringView path, int sample_rate, u16 num_channels, u16 bits_per_sample)
        : m_sample_rate(sample_rate), m_num_channels(num_channels), m_bits_per_sample(bits_per_sample)
    {
        set_file(path);
    }

    /**
     * @brief Construct a new WavWriter::WavWriter object
     * 
     * @param sample_rate 
     * @param num_channels 
     * @param bits_per_sample 
     */
    WavWriter::WavWriter(int sample_rate, u16 num_channels, u16 bits_per_sample)
        : m_sample_rate(sample_rate), m_num_channels(num_channels), m_bits_per_sample(bits_per_sample)
    {
    }

    /**
     * @brief Destroy the WavWriter::WavWriter object
     * 
     */
    WavWriter::~WavWriter()
    {
        if(!m_finalized)
            finalize();
    }

    /**
     * @param path 
     */
    void WavWriter::set_file(StringView path)
    {
        m_file = Core::File::construct(path);
        if(!m_file->open(Core::OpenMode::ReadWrite))
        {
            m_error_string = String::formatted("Can't open file: {}", m_file->error_string());
            return;
        }

        m_file->seek(44);
        m_finalized = false;
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

        if(m_file)
        {
            m_file->seek(0);
            write_header();
            m_file->close();
        }

        m_data_sz = 0;
    }

    void WavWriter::write_header()
    {
        static u32 riff = 0x46464952;
        m_file->write(reinterpret_cast<u8*>(&riff), sizeof(riff));

        u32 sz = m_data_sz + (44 - 4 - 4);
        m_file->write(reinterpret_cast<u8*>(&sz), sizeof(sz));

        static u32 wave = 0x45564157;
        m_file->write(reinterpret_cast<u8*>(&wave), sizeof(wave));

        static u32 fmt_id = 0x20746D66;
        m_file->write(reinterpret_cast<u8*>(&fmt_id), sizeof(fmt_id));

        static u32 fmt_size = 16;
        m_file->write(reinterpret_cast<u8*>(&fmt_size), sizeof(fmt_size));

        static u16 audio_format = 1;
        m_file->write(reinterpret_cast<u8*>(&audio_format), sizeof(audio_format));

        m_file->write(reinterpret_cast<u8*>(&m_num_channels), sizeof(m_num_channels));

        m_file->write(reinterpret_cast<u8*>(&m_sample_rate), sizeof(m_sample_rate));

        u32 byte_rate = m_sample_rate * m_num_channels * (m_bits_per_sample / 8);
        m_file->write(reinterpret_cast<u8*>(&byte_rate), sizeof(byte_rate));

        u16 block_align = m_num_channels * (m_bits_per_sample / 8);
        m_file->write(reinterpret_cast<u8*>(&block_align), sizeof(block_align));

        m_file->write(reinterpret_cast<u8*>(&m_bits_per_sample), sizeof(m_bits_per_sample));

        static u32 chunk_id = 0x61746164;
        m_file->write(reinterpret_cast<u8*>(&chunk_id), sizeof(chunk_id));

        m_file->write(reinterpret_cast<u8*>(&m_data_sz), sizeof(m_data_sz));
    }

} // namespace Audio
