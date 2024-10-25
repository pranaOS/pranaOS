/**
 * @file anonymousbuffer.h
 * @author Krisna Pranav
 * @brief Anonymous Buffer
 * @version 6.0
 * @date 2024-10-25
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <libipc/forward.h>
#include <mods/error.h>
#include <mods/noncopyable.h>
#include <mods/refcounted.h>
#include <mods/refptr.h>
#include <mods/types.h>

namespace Core
{
    class AnonymousBufferImpl final : public RefCounted<AnonymousBufferImpl> 
    {
    public:
        /**
         * @param fd 
         * @return ErrorOr<NonnullRefPtr<AnonymousBufferImpl>> 
         */
        static ErrorOr<NonnullRefPtr<AnonymousBufferImpl>> create(int fd, size_t);

        /**
         * @brief Destroy the Anonymous Buffer Impl object
         * 
         */
        ~AnonymousBufferImpl();

        int fd() const 
        { 
            return m_fd; 
        }

        size_t size() const 
        { 
            return m_size; 
        }

        void* data() 
        { 
            return m_data; 
        }

        const void* data() const 
        { 
            return m_data; 
        }

    private:
        /**
         * @brief Construct a new Anonymous Buffer Impl object
         * 
         * @param fd 
         */
        AnonymousBufferImpl(int fd, size_t, void*);

        int m_fd { -1 };
        size_t m_size { 0 };
        void* m_data { nullptr };
    }; // class AnonymousBufferImpl final : public RefCounted<AnonymousBufferImpl> 
} // namespace Core