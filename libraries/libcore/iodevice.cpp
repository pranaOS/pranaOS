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
} // namespace Core
