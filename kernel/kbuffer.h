/**
 * @file kbuffer.h
 * @author Krisna Pranav
 * @brief kbuffer
 * @version 1.0
 * @date 2023-08-10
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/assertions.h>
#include <mods/byte_buffer.h>
#include <mods/logstream.h>
#include <mods/string_view.h>
#include <kernel/vm/memorymanager.h>
#include <kernel/vm/region.h>
#include <mods/memory.h> 

namespace Kernel 
{

    class KBufferImpl : public RefCounted<KBufferImpl> 
    {
    public:
        /**
         * @param size 
         * @param access 
         * @param name 
         * @return NonnullRefPtr<KBufferImpl> 
         */
        static NonnullRefPtr<KBufferImpl> create_with_size(size_t size, u8 access, const char* name)
        {
            auto region = MM.allocate_kernel_region(PAGE_ROUND_UP(size), name, access, false, false);
            ASSERT(region);

            return adopt(*new KBufferImpl(region.release_nonnull(), size));
        }

        /**
         * @param data 
         * @param size 
         * @param access 
         * @param name 
         * @return NonnullRefPtr<KBufferImpl> 
         */
        static NonnullRefPtr<KBufferImpl> copy(const void* data, size_t size, u8 access, const char* name)
        {
            auto buffer = create_with_size(size, access, name);
            buffer->region().commit();
            memcpy(buffer->data(), data, size);

            return buffer;
        }

        /**
         * @return u8* 
         */
        u8* data() 
        { 
            return m_region->vaddr().as_ptr(); 
        }

        /**
         * @return const u8* 
         */
        const u8* data() const 
        { 
            return m_region->vaddr().as_ptr(); 
        }

        /**
         * @return size_t 
         */
        size_t size() const 
        { 
            return m_size; 
        }

        /**
         * @return size_t 
         */
        size_t capacity() const 
        { 
            return m_region->size(); 
        }

        /**
         * @param size 
         */
        void set_size(size_t size)
        {
            ASSERT(size <= capacity());
            m_size = size;
        }   

        /**
         * @return const Region& 
         */
        const Region& region() const 
        { 
            return *m_region; 
        }

        /**
         * @return Region& 
         */
        Region& region() 
        { 
            return *m_region; 
        }

    private:

        /**
         * @param region 
         * @param size 
         */
        explicit KBufferImpl(NonnullOwnPtr<Region>&& region, size_t size)
            : m_size(size)
            , m_region(move(region))
        {
        }

        size_t m_size { 0 };
        NonnullOwnPtr<Region> m_region;
    }; // class KBufferImpl

    class KBuffer 
    {
    public:

        /**
         * @param size 
         * @param access 
         * @param name 
         * @return KBuffer 
         */
        static KBuffer create_with_size(size_t size, u8 access = Region::Access::Read | Region::Access::Write, const char* name = "KBuffer")
        {
            return KBuffer(KBufferImpl::create_with_size(size, access, name));
        }

        /**
         * @param data 
         * @param size 
         * @param access 
         * @param name 
         * @return KBuffer 
         */
        static KBuffer copy(const void* data, size_t size, u8 access = Region::Access::Read | Region::Access::Write, const char* name = "KBuffer")
        {
            return KBuffer(KBufferImpl::copy(data, size, access, name));
        }

        /**
         * @return u8* 
         */
        u8* data() 
        { 
            return m_impl->data(); 
        }

        /**
         * @return const u8* 
         */
        const u8* data() const 
        { 
            return m_impl->data(); 
        }

        /**
         * @return size_t 
         */
        size_t size() const 
        { 
            return m_impl->size(); 
        }

        /**
         * @return size_t 
         */
        size_t capacity() const 
        { 
            return m_impl->capacity(); 
        }

        /**
         * @return void* 
         */
        void* end_pointer() 
        { 
            return data() + size(); 
        }

        /**
         * @return const void* 
         */
        const void* end_pointer() const 
        { 
            return data() + size(); 
        }

        /**
         * @param size 
         */
        void set_size(size_t size) 
        { 
            m_impl->set_size(size); 
        }

        /**
         * @return const KBufferImpl& 
         */
        const KBufferImpl& impl() const 
        { 
            return m_impl; 
        }

        /**
         * @param buffer 
         * @param access 
         * @param name 
         */
        KBuffer(const ByteBuffer& buffer, u8 access = Region::Access::Read | Region::Access::Write, const char* name = "KBuffer")
            : m_impl(KBufferImpl::copy(buffer.data(), buffer.size(), access, name))
        {
        }

    private:
        /**
         * @param impl 
         */
        explicit KBuffer(NonnullRefPtr<KBufferImpl>&& impl)
            : m_impl(move(impl))
        {
        }

        NonnullRefPtr<KBufferImpl> m_impl;
    }; // class KBuffer

    /**
     * @param stream 
     * @param value 
     * @return const LogStream& 
     */
    inline const LogStream& operator<<(const LogStream& stream, const KBuffer& value)
    {
        return stream << StringView(value.data(), value.size());
    }

} // namespace Kernel
