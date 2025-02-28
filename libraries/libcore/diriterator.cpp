/**
 * @file diriterator.cpp
 * @author Krisna Pranav
 * @brief dir iterator
 * @version 6.0
 * @date 2025-02-28
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/vector.h>
#include <libcore/diriterator.h>
#include <errno.h>
#include <unistd.h>

namespace Core
{
    /**
     * @brief Construct a new DirIterator::DirIterator object
     * 
     * @param path 
     * @param flags 
     */
    DirIterator::DirIterator(String path, Flags flags)
        : m_path(move(path))
        , m_flags(flags)
    {
        m_dir = opendir(m_path.characters());

        if (!m_dir) {
            m_error = errno;
        }
    }

    /**
     * @brief Destroy the DirIterator::DirIterator object
     * 
     */
    DirIterator::~DirIterator()
    {
        if (m_dir) {
            closedir(m_dir);
            m_dir = nullptr;
        }
    }
    
} // namespace Core
