/**
 * @file diriterator.h
 * @author Krisna Pranav
 * @brief dir iterator
 * @version 6.0
 * @date 2023-09-23
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/string.h>
#include <dirent.h>
#include <string.h>

namespace Core 
{

    class DirIterator 
    {
    public:

        enum Flags 
        {
            NoFlags = 0x0,
            SkipDots = 0x1,
            SkipParentAndBaseDir = 0x2,
        }; // enum Flags    

        /**
         * @brief Construct a new DirIterator object
         * 
         * @param path 
         */
        explicit DirIterator(String path, Flags = Flags::NoFlags);

        /**
         * @brief Destroy the DirIterator object
         * 
         */
        ~DirIterator();

        /**
         * @brief Construct a new DirIterator object
         * 
         */
        DirIterator(DirIterator&&);

        /**
         * @brief Construct a new DirIterator object
         * 
         */
        DirIterator(DirIterator const&) = delete;

        /**
         * @return true 
         * @return false 
         */
        bool has_error() const 
        { 
            return m_error != 0; 
        }

        /**
         * @return int 
         */
        int error() const 
        { 
            return m_error; 
        }

        /**
         * @return char const* 
         */
        char const* error_string() const 
        { 
            return strerror(m_error); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool has_next();

        /**
         * @return String 
         */
        String next_path();
        String next_full_path();

        /**
         * @return int 
         */
        int fd() const;

    private:
        DIR* m_dir = nullptr;
        int m_error = 0;
        String m_next;
        String m_path;
        int m_flags;

        bool advance_next();
    }; // class DirIterator

    /**
     * @param filename 
     * @return String 
     */
    String find_executable_in_path(String filename);

} // namespace Core
