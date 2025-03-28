/**
 * @file anonymousbuffer.h
 * @author Krisna Pranav
 * @brief Anonymous Buffer
 * @version 6.0
 * @date 2024-10-25
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/error.h>
#include <mods/noncopyable.h>
#include <mods/refcounted.h>
#include <mods/refptr.h>
#include <mods/types.h>
#include <libipc/forward.h>

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
         * @brief Destroy the AnonymousBufferImpl object
         * 
         */
        ~AnonymousBufferImpl();

        /**
         * @return int 
         */
        int fd() const 
        { 
            return m_fd; 
        }

        /**
         * @return size_t 
         */
        size_t size() const 
        { 
            return m_size; 
        }
        
        void* data() 
        { 
            return m_data; 
        }

        void const* data() const 
        { 
            return m_data; 
        }

    private:
        /**
         * @brief Construct a new AnonymousBufferImpl object
         * 
         * @param fd 
         */
        AnonymousBufferImpl(int fd, size_t, void*);

        int m_fd { -1 };
        size_t m_size { 0 };
        void* m_data { nullptr };
    }; // class AnonymousBufferImpl final : public RefCounted<AnonymousBufferImpl> 

    class AnonymousBuffer 
    {
    public:
        /**
         * @brief Create a with size object
         * 
         * @return ErrorOr<AnonymousBuffer> 
         */
        static ErrorOr<AnonymousBuffer> create_with_size(size_t);

        /**
         * @brief Create a fromanonfd object
         * 
         * @param fd 
         * @return ErrorOr<AnonymousBuffer> 
         */
        static ErrorOr<AnonymousBuffer> create_from_anon_fd(int fd, size_t);

        /**
         * @brief Construct a new AnonymousBuffer object
         * 
         */
        AnonymousBuffer() = default;

        bool is_valid() const 
        { 
            return m_impl; 
        }

        /**
         * @return int 
         */
        int fd() const 
        { 
            return m_impl ? m_impl->fd() : -1; 
        }

        /**
         * @return size_t 
         */
        size_t size() const 
        { 
            return m_impl ? m_impl->size() : 0; 
        }

        /**
         * @tparam T 
         * @return T* 
         */
        template<typename T>
        T* data()
        {
            static_assert(IsVoid<T> || IsTrivial<T>);
            if (!m_impl)
                return nullptr;
            return (T*)m_impl->data();
        }

        /**
         * @tparam T 
         * @return const T* 
         */
        template<typename T>
        const T* data() const
        {
            static_assert(IsVoid<T> || IsTrivial<T>);
            if (!m_impl)
                return nullptr;
            return (const T*)m_impl->data();
        }

    private:
        /**
         * @brief Construct a new Anonymous Buffer object
         * 
         */
        explicit AnonymousBuffer(NonnullRefPtr<AnonymousBufferImpl>);

        RefPtr<AnonymousBufferImpl> m_impl;
    }; // class AnonymousBuffer

} // namespace Core

namespace IPC 
{
    /**
     * @return true 
     * @return false 
     */
    bool encode(Encoder&, Core::AnonymousBuffer const&);

    /**
     * @return ErrorOr<void> 
     */
    ErrorOr<void> decode(Decoder&, Core::AnonymousBuffer&);

} // namespace IPC
