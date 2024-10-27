/**
 * @file mappedfile.h
 * @author Krisna Pranav
 * @brief Mapped File
 * @version 6.0
 * @date 2024-10-27
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/error.h>
#include <mods/noncopyable.h>
#include <mods/nonnullrefptr.h>
#include <mods/refcounted.h>
#include <mods/result.h>

namespace Core {

    class MappedFile : public RefCounted<MappedFile> 
    {
        MOD_MAKE_NONCOPYABLE(MappedFile);
        MOD_MAKE_NONMOVABLE(MappedFile);

    public:
        /**
         * @param path 
         * @return ErrorOr<NonnullRefPtr<MappedFile>> 
         */
        static ErrorOr<NonnullRefPtr<MappedFile>> map(String const& path);

        /**
         * @param fd 
         * @param path 
         * @return ErrorOr<NonnullRefPtr<MappedFile>> 
         */
        static ErrorOr<NonnullRefPtr<MappedFile>> map_from_fd_and_close(int fd, String const& path);

        /**
         * @brief Destroy the Mapped File object
         * 
         */
        ~MappedFile();

        void* data() 
        { 
            return m_data; 
        }

        const void* data() const 
        { 
            return m_data; 
        }

        size_t size() const 
        { 
            return m_size; 
        }

        ReadonlyBytes bytes() const 
        { 
            return { m_data, m_size }; 
        }

    private:
        explicit MappedFile(void*, size_t);

        void* m_data { nullptr };
        size_t m_size { 0 };
    }; // class MappedFile : public RefCounted<MappedFile>
} // namespace Core
