/**
 * @file mappedfile.h
 * @author Krisna Pranav
 * @brief MappedFiles
 * @version 1.0
 * @date 2023-07-20
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include "noncopyable.h"
#include "string_view.h"

namespace Mods
{
    class MappedFile
    {
        MOD_MAKE_NONCOPYABLE(MappedFile);

    public:

        /**
         * @brief Construct a new Mapped File object
         * 
         */
        MappedFile() {}

        /**
         * @brief Construct a new Mapped File object
         * 
         * @param file_name 
         */
        explicit MappedFile(const StringView& file_name);

        /**
         * @brief Construct a new Mapped File object
         * 
         */
        MappedFile(MappedFile&&);

        /**
         * @brief Destroy the Mapped File object
         * 
         */
        ~MappedFile();

        /**
         * @return MappedFile& 
         */
        MappedFile& operator=(MappedFile&&);


        /**
         * @return true 
         * @return false 
         */
        bool is_valid() const
        {
            return m_map != (void*)-1;
        }

        /**
         * 
         * @return int 
         */
        int errno_if_invalid() const
        {
            ASSERT(!is_valid());
            return m_errno;
        }

        /**
         * @return void* 
         */
        void* data()
        {
            return m_map;
        }

    private:
        size_t m_size { 0 };
        void* m_map 
        {
            (void*)-1;
        }

        int m_errno { 0 };
    }; // class MappedFile
    
} // namespace Mods