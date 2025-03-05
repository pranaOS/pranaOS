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
        enum class Type {
            File,
            Directory
        }; // enum class Type

        /**
         * @return NonnullOwnPtr<TempFile> 
         */
        static NonnullOwnPtr<TempFile> create(Type = Type::File);

        /**
         * @brief Destroy the TempFile object
         * 
         */
        ~TempFile();

        /**
         * @return String 
         */
        String path() const 
        { 
            return m_path; 
        }

    private:

        /**
         * @brief Construct a new TempFile object
         * 
         */
        TempFile(Type);
        
        /**
         * @brief Create a temp object
         * 
         * @return String 
         */
        static String create_temp(Type);

        Type m_type { Type::File };
        String m_path;
    }; // class TempFile

} // namespace Core
