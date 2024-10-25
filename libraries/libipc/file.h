/**
 * @file file.h
 * @author Krisna Pranav
 * @brief File
 * @version 6.0
 * @date 2024-10-25
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <unistd.h>
#include <mods/noncopyable.h>
#include <mods/stdlibextra.h>

namespace IPC 
{
    class File 
    {
        MOD_MAKE_NONCOPYABLE(File);

    public:
        /**
         * @brief Construct a new File object
         * 
         */
        File() = default;

        /**
         * @brief Construct a new File object
         * 
         * @param fd 
         */
        File(int fd)
            : m_fd(fd)
        {
        }

        enum Tag 
        {
            ConstructWithReceivedFileDescriptor = 1,
            CloseAfterSending = 1,
        }; // enum Tag

        /**
         * @brief Construct a new File object
         * 
         * @param fd 
         */
        File(int fd, Tag)
            : m_fd(fd)
            , m_close_on_destruction(true)
        {
        }

        /**
         * @brief Construct a new File object
         * 
         * @param other 
         */
        File(File&& other)
            : m_fd(exchange(other.m_fd, -1))
            , m_close_on_destruction(exchange(other.m_close_on_destruction, false))
        {
        }

        /**
         * @param other 
         * @return File& 
         */
        File& operator=(File&& other)
        {
            if (this != &other) {
                m_fd = exchange(other.m_fd, -1);
                m_close_on_destruction = exchange(other.m_close_on_destruction, false);
            }
            return *this;
        }

        /**
         * @brief Destroy the File object
         * 
         */
        ~File()
        {
            if (m_close_on_destruction && m_fd != -1)
                close(m_fd);
        }

        int fd() const 
        { 
            return m_fd; 
        }

        [[nodiscard]] int take_fd() const
        {
            return exchange(m_fd, -1);
        }

    private:
        mutable int m_fd { -1 };
        bool m_close_on_destruction { false };
    }; // class File
} // namespace IPC
