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
     * @brief Construct a new DirIterator:: DirIterator object
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

    /**
     * @brief Construct a new DirIterator::DirIterator object
     * 
     * @param other 
     */
    DirIterator::DirIterator(DirIterator&& other)
        : m_dir(other.m_dir)
        , m_error(other.m_error)
        , m_next(move(other.m_next))
        , m_path(move(other.m_path))
        , m_flags(other.m_flags)
    {
        other.m_dir = nullptr;
    }

    /**
     * @return true 
     * @return false 
     */
    bool DirIterator::advance_next()
    {
        if (!m_dir)
            return false;

        while (true) {
            errno = 0;
            auto* de = readdir(m_dir);
            if (!de) {
                m_error = errno;
                m_next = String();
                return false;
            }

            m_next = de->d_name;
            if (m_next.is_null())
                return false;

            if (m_flags & Flags::SkipDots && m_next.starts_with('.'))
                continue;

            if (m_flags & Flags::SkipParentAndBaseDir && (m_next == "." || m_next == ".."))
                continue;

            return !m_next.is_empty();
        }
    }

    /**
     * @return true 
     * @return false 
     */
    bool DirIterator::has_next()
    {
        if (!m_next.is_null())
            return true;

        return advance_next();
    }

    /**
     * @return String 
     */
    String DirIterator::next_path()
    {
        if (m_next.is_null())
            advance_next();

        auto tmp = m_next;
        m_next = String();
        return tmp;
    }

    /**
     * @return String 
     */
    String DirIterator::next_full_path()
    {
        StringBuilder builder;
        builder.append(m_path);

        if (!m_path.ends_with('/'))
            builder.append('/');

        builder.append(next_path());
        return builder.to_string();
    }

    /**
     * @param filename 
     * @return String 
     */
    String find_executable_in_path(String filename)
    {
        if (filename.is_empty())
            return {};

        if (filename.starts_with('/')) {
            if (access(filename.characters(), X_OK) == 0)
                return filename;

            return {};
        }

        for (auto directory : String { getenv("PATH") }.split(':')) {
            auto fullpath = String::formatted("{}/{}", directory, filename);

            if (access(fullpath.characters(), X_OK) == 0)
                return fullpath;
        }

        return {};
    }

    /**
     * @return int 
     */
    int DirIterator::fd() const
    {
        if (!m_dir)
            return -1;
        return dirfd(m_dir);
    }

} // namespace Core
