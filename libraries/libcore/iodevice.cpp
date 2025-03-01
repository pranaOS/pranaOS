/**
 * @file iodevice.cpp
 * @author Krisna Pranav
 * @brief iodevice
 * @version 6.0
 * @date 2025-03-01
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/bytebuffer.h>
#include <libcore/iodevice.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>

namespace Core
{

    /**
     * @brief Construct a new IODevice::IODevice object
     * 
     * @param parent 
     */
    IODevice::IODevice(Object* parent)
        : Object(parent)
    {}

    /**
     * @return char const* 
     */
    char const* IODevice::error_string() const
    {
        return strerror(m_error);
    }

    /**
     * @param buffer 
     * @param length 
     * @return int 
     */
    int IODevice::read(u8* buffer, int length)
    {
        auto read_buffer = read(length);
        memcpy(buffer, read_buffer.data(), length);

        return read_buffer.size();
    }

    /**
     * @param offset 
     * @param mode 
     * @param pos 
     * @return true 
     * @return false 
     */
    bool IODevice::seek(i64 offset, SeekMode mode, off_t* pos)
    {
        int m = SEEK_SET;

        switch (mode) {
        case SeekMode::SetPosition:
            m = SEEK_SET;
            break;
        case SeekMode::FromCurrentPosition:
            m = SEEK_CUR;
            offset -= m_buffered_data.size();
            break;
        case SeekMode::FromEndPosition:
            m = SEEK_END;
            break;
        }

        off_t rc = lseek(m_fd, offset, m);
        
        if (rc < 0) {
            set_error(errno);
            if (pos)
                *pos = -1;
            return false;
        }
        
        m_buffered_data.clear();
        m_eof = false;

        if (pos)
            *pos = rc;

        return true;
    }

    /**
     * @param size 
     * @return true 
     * @return false 
     */
    bool IODevice::truncate(off_t size)
    {
        int rc = ftruncate(m_fd, size);

        if (rc < 0) {
            set_error(errno);
            return false;
        }

        return true;
    }

    /**
     * @param data 
     * @param size 
     * @return true 
     * @return false 
     */
    bool IODevice::write(u8 const* data, int size)
    {
        int rc = ::write(m_fd, data, size);

        if (rc < 0) {
            set_error(errno);
            perror("IODevice::write: write");
            return false;
        }

        return rc == size;
    }

    /**
     * @param fd 
     */
    void IODevice::set_fd(int fd)
    {
        if (m_fd == fd)
            return;

        m_fd = fd;
        did_update_fd(fd);
    }

    /**
     * @param v 
     * @return true 
     * @return false 
     */
    bool IODevice::write(StringView v)
    {
        return write((u8 const*)v.characters_without_null_termination(), v.length());
    }
} // namespace Core
