/**
 * @file stdio_file_implementation.h
 * @author Krisna Pranav
 * @brief stdio file implementation
 * @version 6.0
 * @date 2025-02-21
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <libc/bits/FILE.h>

#pragma once

struct File
{

}; // struct File

class ScopedFileLock 
{
public:
    /**
     * @brief Construct a new ScopedFileLock object
     * 
     * @param file 
     */
    ScopedFileLock(FILE* file)
        : m_file(file)
    {
        m_file->lock();
    }

    ~ScopedFileLock()
    {
        m_file->unlocked();
    }

private:
    FILE* file;
}; // class ScopedFileLock 