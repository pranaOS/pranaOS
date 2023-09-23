/**
 * @file diriterator.h
 * @author Krisna Pranav
 * @brief dir iterator
 * @version 6.0
 * @date 2023-09-23
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
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
         * @param path 
         */
        DirIterator(const StringView& path, Flags = Flags::NoFlags);

        /// @brief Destroy the Dir Iterator object
        ~DirIterator();

        /**
         * @return true 
         * @return false 
         */
        bool has_error() const { return m_error != 0; }

        /**
         * @return int 
         */
        int error() const 
        { 
            return m_error; 
        }

        /**
         * @return const char* 
         */
        const char* error_string() const 
        { 
            return strerror(m_error); 
        }

        bool has_next();
        
        String next_path();
        String next_full_path();

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