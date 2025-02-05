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
#   include <errno.h>
#   include <stdio.h>

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


    private:
        FILE* m_file { nullptr };
        bool m_owned { false };
    };  // class InputFileStream : public InputStream
} // namespace Mods