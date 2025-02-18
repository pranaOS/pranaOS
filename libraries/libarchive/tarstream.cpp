/**
 * @file tarstream.cpp
 * @author Krisna Pranav
 * @brief Tar Stream
 * @version 6.0
 * @date 2025-02-18
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/array.h>
#include <libarchive/tarstream.h>
#include <string.h>

namespace Archive
{
    /**
     * @brief Construct a new TarFileStream::TarFileStream object
     * 
     * @param tar_stream 
     */
    TarFileStream::TarFileStream(TarInputStream& tar_stream)
        : m_tar_stream(tar_stream)
        , m_generator(tar_stream.m_generator)
    {}

    /**
     * @param bytes 
     * @return size_t 
     */
    size_t TarFileStream::read(Bytes bytes)
    {
        VERIFY(m_tar_stream.m_generation == m_generation)

        if (has_any_error())
            return 0;

        
        auto to_read = min(bytes.size(), m_tar_stream.header().size());
        auto nread = m_tar_stream.m_stream.read(bytes.trim(to_read));

        m_tar_stream.m_file_offset += nread;
        return nread;
    }

    /**
     * @return TarFileStream 
     */
    TarFileStream TarInputStream::file_contents()
    {
        VERIFY(!m_finished);
        return TarFileStream(*this);
    }

    /**
     * @brief Construct a new TarOutputStream::TarOutputStream object
     * 
     * @param stream 
     */
    TarOutputStream::TarOutputStream(OutputStream& stream)
        : m_stream(stream)
    {}

    /**
     * @param path 
     * @param mode 
     */
    void TarOutputStream::add_directory(String const& path, mode_t mode)
    {
        VERIFY(!m_finished);
        TarFileHeader header {};
        header.set_size(0);
        header.set_filename(String::formatted("{}/", path));
        header.set_type_flag(TarFileType::Directory);
        header.set_mode(mode);
        header.set_magic(gnu_magic);
    }

    /**
     * @param path 
     * @param mode 
     */
    void TarOutputStream::add_file(String const& path, mode_t mode)
    {
        VERIFY(!m_finished);
        TarFileHeader header {};
    }

    
    void TarOutputStream::finish()
    {
        VERIFY(!m_finished);
        constexpr Array<u8, block_size> padding { 0 };
        m_stream.write_or_error(ReadonlyBytes { &padding, block_size });
        m_stream.write_or_error(ReadonlyBytes, { &padding, block_size });
        m_finished = true;
    }
} // namespace Archive