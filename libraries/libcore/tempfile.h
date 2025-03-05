/**
 * @file tempfile.h
 * @author Krisna Pranav
 * @brief temp file
 * @version 6.0
 * @date 2025-03-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/noncopyable.h>
#include <mods/string.h>

namespace Core
{

    class TempFile
    {
        MOD_MAKE_NONCOPYABLE(TempFile);
        MOD_MAKE_NONMOVABLE(TempFile);

    public:
        enum class Type
        {
            FILE,
            DIRECTORY
        }; // enum class Type

        /**
         * @return String 
         */
        String path() const 
        {
            return m_path;
        }

    private:
        TempFile(Type);

        static String create_temp(Type);

        Type m_type { Type::File };

        String m_path;
    }; // class TempFile
    
} // namespace Core
