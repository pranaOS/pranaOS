/**
 * @file filestream.h
 * @author Krisna Pranav
 * @brief File Stream
 * @version 6.0
 * @date 2025-02-28
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/buffered.h>
#include <mods/bytebuffer.h>
#include <mods/stream.h>
#include <mods/try.h>
#include <libcore/file.h>

namespace Core 
{

    class InputFileStream final : public InputStream 
    {
    public:
        /**
         * @brief Construct a new InputFileStream object
         * 
         * @param file 
         */
        explicit InputFileStream(NonnullRefPtr<File> file)
            : m_file(file)
        {
        }

        /**
         * @param filename 
         * @param mode 
         * @param permissions 
         * @return ErrorOr<InputFileStream> 
         */
        static ErrorOr<InputFileStream> open(StringView filename, OpenMode mode = OpenMode::ReadOnly, mode_t permissions = 0644)
        {
            VERIFY(has_flag(mode, OpenMode::ReadOnly));
            auto file = TRY(File::open(filename, mode, permissions));

            return InputFileStream { move(file) };
        }

        /**
         * @param filename 
         * @param mode 
         * @param permissions 
         * @return ErrorOr<Buffered<InputFileStream>> 
         */
        static ErrorOr<Buffered<InputFileStream>> open_buffered(StringView filename, OpenMode mode = OpenMode::ReadOnly, mode_t permissions = 0644)
        {
            VERIFY(has_flag(mode, OpenMode::ReadOnly));
            auto file = TRY(File::open(filename, mode, permissions));

            return Buffered<InputFileStream> { move(file) };
        }

        /**
         * @param bytes 
         * @return size_t 
         */
        size_t read(Bytes bytes) override
        {
            if (has_any_error())
                return 0;

            auto const buffer = m_file->read(bytes.size());

            return buffer.bytes().copy_to(bytes);
        }

        /**
         * @param bytes 
         * @return true 
         * @return false 
         */
        bool read_or_error(Bytes bytes) override
        {
            if (read(bytes) < bytes.size()) {
                set_fatal_error();
                return false;
            }

            return true;
        }

        /**
         * @param offset 
         * @param whence 
         * @return true 
         * @return false 
         */
        bool seek(size_t offset, SeekMode whence = SeekMode::SetPosition)
        {
            return m_file->seek(offset, whence);
        }

        /**
         * @param count 
         * @return true 
         * @return false 
         */
        bool discard_or_error(size_t count) override 
        { 
            return m_file->seek(count, SeekMode::FromCurrentPosition); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool unreliable_eof() const override 
        { 
            return m_file->eof(); 
        }

        void close()
        {
            if (!m_file->close())
                set_fatal_error();
        }

    private:
        NonnullRefPtr<File> m_file;
    }; // class InputFileStream final : public InputStream

    class OutputFileStream : public OutputStream 
    {
    public:
        /**
         * @brief Construct a new OutputFileStream object
         * 
         * @param file 
         */
        explicit OutputFileStream(NonnullRefPtr<File> file)
            : m_file(file)
        {
        }

        /**
         * @param filename 
         * @param mode 
         * @param permissions 
         * @return ErrorOr<OutputFileStream> 
         */
        static ErrorOr<OutputFileStream> open(StringView filename, OpenMode mode = OpenMode::WriteOnly, mode_t permissions = 0644)
        {
            VERIFY(has_flag(mode, OpenMode::WriteOnly));
            auto file = TRY(File::open(filename, mode, permissions));

            return OutputFileStream { move(file) };
        }

        /**
         * @param filename 
         * @param mode 
         * @param permissions 
         * @return ErrorOr<Buffered<OutputFileStream>> 
         */
        static ErrorOr<Buffered<OutputFileStream>> open_buffered(StringView filename, OpenMode mode = OpenMode::WriteOnly, mode_t permissions = 0644)
        {
            VERIFY(has_flag(mode, OpenMode::WriteOnly));
            auto file = TRY(File::open(filename, mode, permissions));

            return Buffered<OutputFileStream> { move(file) };
        }

        /**
         * @return OutputFileStream 
         */
        static OutputFileStream standard_output()
        {
            return OutputFileStream { Core::File::standard_output() };
        }   

        /**
         * @return OutputFileStream 
         */
        static OutputFileStream standard_error()
        {
            return OutputFileStream { Core::File::standard_error() };
        }

        /**
         * @return Buffered<OutputFileStream> 
         */
        static Buffered<OutputFileStream> stdout_buffered()
        {
            return Buffered<OutputFileStream> { Core::File::standard_output() };
        }

        /**
         * @param bytes 
         * @return size_t 
         */
        size_t write(ReadonlyBytes bytes) override
        {
            if (!m_file->write(bytes.data(), bytes.size())) {
                set_fatal_error();
                return 0;
            }

            return bytes.size();
        }

        /**
         * @param bytes 
         * @return true 
         * @return false 
         */
        bool write_or_error(ReadonlyBytes bytes) override
        {
            if (write(bytes) < bytes.size()) {
                set_fatal_error();
                return false;
            }

            return true;
        }

        void close()
        {
            if (!m_file->close())
                set_fatal_error();
        }

    private:
        NonnullRefPtr<File> m_file;
    }; // class OutputFileStream : public OutputStream

} // namespace Core
