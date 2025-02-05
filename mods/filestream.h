/**
 * @file filestream.h
 * @author Krisna Pranav
 * @brief File Stream
 * @version 6.0
 * @date 2025-02-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/stream.h>
#ifndef KERNEL
#    include <errno.h>
#    include <stdio.h>

namespace Mods
{
    class InputFileStream : public InputStream 
    {
    public:
        /**
         * @brief Construct a new Input File Stream object
         * 
         * @param fd 
         */
        explicit InputFileStream(int fd)
            : m_file(fdopen(fd, "r"))
            , m_owned(true)
        {
            if (!m_file)
                set_fatal_error();
        }

        /**
         * @brief Construct a new Input File Stream object
         * 
         * @param fp 
         */
        explicit InputFileStream(FILE* fp)
            : m_file(fp)
        {
            if (!m_file)
                set_fatal_error();
        }

        /**
         * @brief Destroy the Input File Stream object
         * 
         */
        ~InputFileStream()
        {
            if (m_file) {
                fflush(m_file);
                if (m_owned)
                    fclose(m_file);
            }
        }

        /**
         * @return true 
         * @return false 
         */
        bool unreliable_eof() const override 
        { 
            return eof(); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool eof() const 
        { 
            return feof(m_file); 
        }

        /**
         * @param bytes 
         * @return size_t 
         */
        size_t read(Bytes bytes) override
        {
            if (has_any_error())
                return 0;

            return fread(bytes.data(), sizeof(u8), bytes.size(), m_file);
        }

        /**
         * @param bytes 
         * @return true 
         * @return false 
         */
        bool read_or_error(Bytes bytes) override
        {
            if (has_any_error())
                return false;

            auto size = read(bytes);

            if (size < bytes.size()) {
                set_recoverable_error();
                return false;
            }

            return true;
        }

        /**
         * @param count 
         * @return true 
         * @return false 
         */
        bool discard_or_error(size_t count) override
        {
            if (fseek(m_file, count, SEEK_CUR) == 0)
                return true;

            if (errno != ESPIPE)
                return false;

            char buf[4];
            size_t i = 0;

            while (i < count) {
                auto size = min(count - i, 4ul);
                if (read({ buf, size }) < size) {
                    return false;
                }
                i += size;
            }

            return true;
        }

        /**
         * @param offset 
         * @param whence 
         * @return true 
         * @return false 
         */
        bool seek(size_t offset, int whence = SEEK_SET)
        {
            return fseek(m_file, offset, whence) == 0;
        }

        virtual bool handle_any_error() override
        {
            clearerr(m_file);
            return Stream::handle_any_error();
        }

        void make_unbuffered()
        {
            setvbuf(m_file, nullptr, _IONBF, 0);
        }

    private:
        FILE* m_file { nullptr };
        bool m_owned { false };
    }; // class InputFileStream : public InputStream

    class OutputFileStream : public OutputStream 
    {
    public:
        /**
         * @brief Construct a new Output File Stream object
         * 
         * @param fd 
         */
        explicit OutputFileStream(int fd)
            : m_file(fdopen(fd, "w"))
            , m_owned(true)
        {
            if (!m_file)
                set_fatal_error();
        }

        /**
         * @brief Construct a new Output File Stream object
         * 
         * @param fp 
         */
        explicit OutputFileStream(FILE* fp)
            : m_file(fp)
        {
            if (!m_file)
                set_fatal_error();
        }

        /**
         * @brief Destroy the Output File Stream object
         * 
         */
        ~OutputFileStream()
        {
            if (m_file) {
                fflush(m_file);
                if (m_owned)
                    fclose(m_file);
            }
        }

        /**
         * @param bytes 
         * @return size_t 
         */
        size_t write(ReadonlyBytes bytes) override
        {
            auto nwritten = fwrite(bytes.data(), sizeof(u8), bytes.size(), m_file);
            m_bytes_written += nwritten;
            return nwritten;
        }

        /**
         * @param bytes 
         * @return true 
         * @return false 
         */
        bool write_or_error(ReadonlyBytes bytes) override
        {
            auto nwritten = write(bytes);
            if (nwritten < bytes.size()) {
                set_recoverable_error();
                return false;
            }
            return true;
        }

        /**
         * @return size_t 
         */
        size_t size() const 
        { 
            return m_bytes_written; 
        }

        virtual bool handle_any_error() override
        {
            clearerr(m_file);
            return Stream::handle_any_error();
        }

        void make_unbuffered()
        {
            setvbuf(m_file, nullptr, _IONBF, 0);
        }

    private:
        FILE* m_file { nullptr };
        size_t m_bytes_written { 0 };
        bool m_owned { false };
    }; // class OutputFileStream : public OutputStream
} // namespace Mods

using Mods::InputFileStream;
using Mods::OutputFileStream;

#endif
