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
        /**
         * @brief Construct a new TarFileStream object
         * 
         * @param stream 
         */
        TarFileStream(TarInputStream& stream);

        TarInputStream& m_tar_stream;
        int m_generation;

        friend class TarInputStream;
    }; // class TarFileStream : public InputStream

    class TarInputStream
    {
    public:
        /**
         * @brief Construct a new TarInputStream object
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
        template <VoidFunction<StringView, StringView> F>
        ErrorOr<void> for_each_extended_header(F func);

    private:
        TarFileHeader m_header;
        InputStream& m_stream;
        unsigned long m_file_offset{0};
        int m_generation{0};
        bool m_finished{false};

        friend class TarFileStream;
    }; // class TarInputStream;

    class TarOutputStream
    {
    public:
        /**
         * @brief Construct a new TarOutputStream object
         * 
         */
        TarOutputStream(OutputStream&);

        /**
         * @param path 
         */
        void add_file(String const& path, mode_t, ReadonlyBytes);

        /**
         * @param path 
         */
        void add_directory(String const& path, mode_t);

        void finish();

    private:
        OutputStream& m_stream;
        bool m_finished{false};

        friend class TarFileStream;
    }; // class TarOutputStream

    /**
     * @tparam F 
     * @param func 
     * @return ErrorOr<void> 
     */
    template <VoidFunction<StringView, StringView> F>
    inline ErrorOr<void> TarInputStream::for_each_extended_header(F func)
    {
        VERIFY(header().content_is_like_extended_header());

        Archive::TarFileStream file_stream = file_contents();

        ByteBuffer file_contents_buffer = TRY(ByteBuffer::create_zeroed(header().size()));

        VERIFY(file_stream.read(file_contents_buffer) == header().size());

        StringView file_contents{file_contents_buffer};

        while(!file_contents.is_empty())
        {
            Optional<size_t> length_end_index = file_contents.find(' ');

            if(!length_end_index.has_value())
                return Error::from_string_literal("Malformed extended header: No length found.");

            Optional<unsigned int> length = file_contents.substring_view(0, length_end_index.value()).to_uint();

            if(!length.has_value())
                return Error::from_string_literal("Malformed extended header: Could not parse length.");

            unsigned int remaining_length = length.value();

            remaining_length -= length_end_index.value() + 1;
            file_contents = file_contents.substring_view(length_end_index.value() + 1);

            StringView header = file_contents.substring_view(0, remaining_length - 1);
            file_contents = file_contents.substring_view(remaining_length - 1);

            if(file_contents.length() < 1 || !file_contents.starts_with('\n'))
                return Error::from_string_literal("Malformed extended header: Header does not end at expected location.");

            file_contents = file_contents.substring_view(1);

            Optional<size_t> header_delimiter_index = header.find('=');

            if(!header_delimiter_index.has_value())
                return Error::from_string_literal("Malformed extended header: Header does not have a delimiter.");

            StringView key = header.substring_view(0, header_delimiter_index.value());
            StringView value = header.substring_view(header_delimiter_index.value() + 1);

            func(key, value);
        }

        return {};
    } // inline ErrorOr<void> TarInputStream::for_each_extended_header(F func)

} // namespace Archive
