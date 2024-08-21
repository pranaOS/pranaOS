/**
 * @file mappedfile.h
 * @author Krisna Pranav
 * @brief MappedFiles
 * @version 6.0
 * @date 2023-07-20
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
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
        MappedFile() { }

        /**
         * @param file_name 
         */
        explicit MappedFile(const StringView& file_name);

        /// @param MappedFile 
        MappedFile(MappedFile&&);
        ~MappedFile();

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
         * @return int 
         */
        int errno_if_invalid() const
        {
            ASSERT(!is_valid());
            return m_errno;
        }

        void unmap();

        /**
         * @return void* 
         */
        void* data() 
        { 
            return m_map; 
        }

        /**
         * @return const void* 
         */
        const void* data() const 
        { 
            return m_map; 
        }

        /**
         * @return size_t 
         */
        size_t size() const 
        { 
            return m_size; 
        }

    private:
        size_t m_size { 0 };
        void* m_map { (void*)-1 };
        int m_errno { 0 };
    }; // class MappedFile

} // namespace Mods

using Mods::MappedFile;