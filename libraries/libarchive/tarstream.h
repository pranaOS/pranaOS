/**
 * @file tarstream.h
 * @author Krisna Pranav
 * @brief Tar Stream
 * @version 6.0
 * @date 2025-02-17
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/span.h>
#include <mods/stream.h>
#include <libarchive/tar.h>

namespace Archive
{
    class TarInputStream;

    class TarFileStream : public InputStream
    {
    public:
        /**
         * @return size_t 
         */
        size_t read(Bytes) override;

        /**
         * @return true 
         * @return false 
         */
        bool unreliable_eof() const override;

        /**
         * @return true 
         * @return false 
         */
        bool read_or_error(Bytes) override;

        /**
         * @param count 
         * @return true 
         * @return false 
         */
        bool discard_or_error(size_t count) override;
    private:
        TarFileStream(TarInputStream& stream);
        TarInputStream& m_tar_stream;
        int m_generator;
        friend class TarInputStream;
    }; // class TarFileStream : public InputStream

    class TarInputStream
    {
    public:
        /**
         * @brief Construct a new Tar Input Stream object
         * 
         */
        TarInputStream(InputStream&);

        void advance();

        /**
         * @return true 
         * @return false 
         */
        bool finished() const
        {
            return m_finished;
        }

        /**
         * @return true 
         * @return false 
         */
        bool valid() const;

        /**
         * @return TarFileHeader const& 
         */
        TarFileHeader const& header() const
        {
            return m_header;
        }

        /**
         * @return TarFileStream 
         */
        TarFileStream file_contents();

        /**
         * @tparam F 
         * @param func 
         * @return ErrorOr<void> 
         */
        template<VoidFunction<StringView, StringView> F>
        ErrorOr<void> for_each_extended_header(F func);

    private:
        TarFileHeader m_header;
        InputStream& m_stream;
        unsigned long m_file_offset { 0 };
        int m_generator { 0 };
        bool m_finished { false };
        friend class TarFileStream;
    }; // class TarInputStream

    class TarOutputStream 
    {

    }; // class TarOutputStream 
} // namespace Archive