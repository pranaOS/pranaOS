/**
 * @file byte_buffer.h
 * @author Krisna Pranav
 * @brief Byte Buffer
 * @version 1.0 
 * @date 2023-07-04
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "nonnullrefptr.h"
#include "refcounted.h"
#include "span.h"
#include "refptr.h"
#include "stdlibextra.h"
#include "kmalloc.h"

namespace Mods {

    class ByteBufferImpl : public RefCounted<ByteBufferImpl> {
    public:

        /**
         * @param size 
         * @return NonnullRefPtr<ByteBufferImpl> 
         */
        static NonnullRefPtr<ByteBufferImpl> create_uninitialized(size_t size);

        /**
         * @return NonnullRefPtr<ByteBufferImpl> 
         */
        static NonnullRefPtr<ByteBufferImpl> create_zeroed(size_t);

        /**
         * @return NonnullRefPtr<ByteBufferImpl> 
         */
        static NonnullRefPtr<ByteBufferImpl> copy(const void*, size_t);

        /**
         * @return NonnullRefPtr<ByteBufferImpl> 
         */
        static NonnullRefPtr<ByteBufferImpl> wrap(void*, size_t);

        /**
         * @brief adopt
         * 
         * @return NonnullRefPtr<ByteBufferImpl> 
         */
        static NonnullRefPtr<ByteBufferImpl> adopt(void*, size_t);

        /**
         * @brief Destroy the Byte Buffer Impl object
         * 
         */
        ~ByteBufferImpl() { 
            clear(); 
        }

        void clear() {
            if (!m_data)
                return;
            if (m_owned)
                kfree(m_data);
            m_data = nullptr;
        }

        /**
         * @param i 
         * @return u8& 
         */
        u8& operator[](size_t i) {
            ASSERT(i < m_size);
            return m_data[i];
        }

        /**
         * @param i 
         * @return const u8& 
         */
        const u8& operator[](size_t i) const {
            ASSERT(i < m_size);
            return m_data[i];
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_empty() const { 
            return !m_size; 
        }

        /**
         * @return size_t 
         */
        size_t size() const { 
            return m_size; 
        }

        /**
         * @return u8* 
         */
        u8* data() { 
            return m_data; 
        }

        /**
         * @return const u8* 
         */
        const u8* data() const { 
            return m_data; 
        }

        /**
         * @return Bytes 
         */
        Bytes bytes() { 
            return { data(), size() }; 
        }

        /**
         * @return ReadonlyBytes 
         */
        ReadonlyBytes bytes() const { 
            return { data(), size() }; 
        }

        /**
         * @param offset 
         * @return u8* 
         */
        u8* offset_pointer(int offset) { 
            return m_data + offset; 
        }

        /**
         * @param offset 
         * @return const u8* 
         */
        const u8* offset_pointer(int offset) const { 
            return m_data + offset; 
        }

        /**
         * @return void* 
         */
        void* end_pointer() { 
            return m_data + m_size; 
        }

        /**
         * @return const void* 
         */
        const void* end_pointer() const { 
            return m_data + m_size; 
        }

        /**
         * @param size 
         */
        void trim(size_t size) {
            ASSERT(size <= m_size);
            m_size = size;
        }

        /**
         * @param size 
         */
        void grow(size_t size);

    private:

        // MODES
        enum ConstructionMode {
            Uninitialized,
            Copy,
            Wrap,
            Adopt
        };

        /**
         * @brief Construct a new Byte Buffer Impl object
         * 
         */
        explicit ByteBufferImpl(size_t);                 

        ByteBufferImpl(const void*, size_t, ConstructionMode); 
        ByteBufferImpl(void*, size_t, ConstructionMode);       

        /**
         * @brief Construct a new Byte Buffer Impl object
         * 
         */
        ByteBufferImpl() {}

        u8* m_data { nullptr };
        size_t m_size { 0 };
        bool m_owned { false };
    };

    class ByteBuffer {
    public:
        ByteBuffer() { }
        ByteBuffer(std::nullptr_t) { }

        /**
         * @brief Construct a new Byte Buffer object
         * 
         * @param other 
         */
        ByteBuffer(const ByteBuffer& other) : m_impl(other.m_impl)
        {
        }

        ByteBuffer(ByteBuffer&& other) : m_impl(move(other.m_impl))
        {
        }

        /**
         * @param other 
         * @return ByteBuffer& 
         */
        ByteBuffer& operator=(ByteBuffer&& other) {
            if (this != &other)
                m_impl = move(other.m_impl);
            return *this;
        }

        /**
         * @param other 
         * @return ByteBuffer& 
         */
        ByteBuffer& operator=(const ByteBuffer& other) {
            if (this != &other)
                m_impl = other.m_impl;
            return *this;
        }

        /**
         * @param size 
         * @return ByteBuffer 
         */
        static ByteBuffer create_uninitialized(size_t size) { 
            return ByteBuffer(ByteBufferImpl::create_uninitialized(size)); 
        }

        /**
         * @param size 
         * @return ByteBuffer 
         */
        static ByteBuffer create_zeroed(size_t size) { 
            return ByteBuffer(ByteBufferImpl::create_zeroed(size)); 
        }

        /**
         * @param data 
         * @param size 
         * @return ByteBuffer 
         */
        static ByteBuffer copy(const void* data, size_t size) { 
            return ByteBuffer(ByteBufferImpl::copy(data, size)); 
        }

        /**
         * @param data 
         * @param size 
         * @return ByteBuffer 
         */
        static ByteBuffer wrap(void* data, size_t size) { 
            return ByteBuffer(ByteBufferImpl::wrap(data, size)); 
        }

        /**
         * @param data 
         * @param size 
         * @return ByteBuffer 
         */
        static ByteBuffer adopt(void* data, size_t size) { 
            return ByteBuffer(ByteBufferImpl::adopt(data, size)); 
        }

        /**
         * @brief Destroy the Byte Buffer object
         * 
         */
        ~ByteBuffer() { 
            clear(); 
        }


        void clear() { 
            m_impl = nullptr; 
        }

        /**
         * @return true 
         * @return false 
         */
        operator bool() const { 
            return !is_null(); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool operator!() const { 
            return is_null(); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_null() const { 
            return m_impl == nullptr; 
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(const ByteBuffer& other) const;

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(const ByteBuffer& other) const { 
            return !(*this == other); 
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator<=(const ByteBuffer& other) const = delete;

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator>=(const ByteBuffer& other) const = delete;

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator<(const ByteBuffer& other) const = delete;

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator>(const ByteBuffer& other) const = delete;

        /**
         * @param i 
         * @return u8& 
         */
        u8& operator[](size_t i) {
            ASSERT(m_impl);
            return (*m_impl)[i];
        }

        /**
         * @param i 
         * @return u8 
         */
        u8 operator[](size_t i) const {
            ASSERT(m_impl);
            return (*m_impl)[i];
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_empty() const { 
            return !m_impl || m_impl->is_empty(); 
        }

        /**
         * @return size_t 
         */
        size_t size() const { 
            return m_impl ? m_impl->size() : 0; 
        }

        /**
         * @return u8* 
         */
        u8* data() { 
            return m_impl ? m_impl->data() : nullptr; 
        }

        /**
         * @return const u8* 
         */
        const u8* data() const { 
            return m_impl ? m_impl->data() : nullptr; 
        }

        /** 
         * @return Bytes 
         */
        Bytes bytes() { 
            return m_impl ? m_impl->bytes() : nullptr; 
        }

        /**
         * @return ReadonlyBytes 
         */
        ReadonlyBytes bytes() const { 
            return m_impl ? m_impl->bytes() : nullptr; 
        }

        /**
         * @param offset 
         * @return u8* 
         */
        u8* offset_pointer(int offset) { 
            return m_impl ? m_impl->offset_pointer(offset) : nullptr; 
        }

        /** 
         * @param offset 
         * @return const u8* 
         */
        const u8* offset_pointer(int offset) const { 
            return m_impl ? m_impl->offset_pointer(offset) : nullptr; 
        }

        /**
         * @return void* 
         */
        void* end_pointer() { 
            return m_impl ? m_impl->end_pointer() : nullptr; 
        }
        
        /**
         * @return const void* 
         */
        const void* end_pointer() const { 
            return m_impl ? m_impl->end_pointer() : nullptr; 
        }

        /**
         * @return ByteBuffer 
         */
        ByteBuffer isolated_copy() const {
            if (!m_impl)
                return {};
            return copy(m_impl->data(), m_impl->size());
        }

        /**
         * @param size 
         */
        void trim(size_t size) {
            if (m_impl)
                m_impl->trim(size);
        }

        /**
         * @param offset 
         * @param size 
         * @return ByteBuffer 
         */
        ByteBuffer slice_view(size_t offset, size_t size) const {
            if (is_null())
                return {};

            ASSERT(offset + size <= this->size());

            return wrap(const_cast<u8*>(offset_pointer(offset)), size);
        }

        /**
         * @param offset 
         * @param size 
         * @return ByteBuffer 
         */
        ByteBuffer slice(size_t offset, size_t size) const {
            if (is_null())
                return {};

            ASSERT(offset + size <= this->size());

            return copy(offset_pointer(offset), size);
        }

        /**
         * @param size 
         */
        void grow(size_t size) {
            if (!m_impl)
                m_impl = ByteBufferImpl::create_uninitialized(size);
            else
                m_impl->grow(size);
        }

        /**
         * @param data 
         * @param data_size 
         */
        void append(const void* data, size_t data_size) {
            if (data_size == 0)
                return;
            ASSERT(data != nullptr);
            int old_size = size();
            grow(size() + data_size);
            __builtin_memcpy(this->data() + old_size, data, data_size);
        }

        /** 
         * @param other 
         */
        void operator+=(const ByteBuffer& other) {
            append(other.data(), other.size());
        }

        /**
         * @param offset 
         * @param data 
         * @param data_size 
         */
        void overwrite(size_t offset, const void* data, size_t data_size) {
            ASSERT(offset + data_size <= size());
            __builtin_memcpy(this->data() + offset, data, data_size);
        }

        /**
         * @return Bytes 
         */
        operator Bytes() { 
            return bytes(); 
        }

        /** 
         * @return ReadonlyBytes 
         */
        operator ReadonlyBytes() const { 
            return bytes(); 
        }

    private:
        /**
         * 
         * @param impl 
         */
        explicit ByteBuffer(RefPtr<ByteBufferImpl>&& impl) : m_impl(move(impl))
        {
        }

        RefPtr<ByteBufferImpl> m_impl;
    };

    /**
     * @param size 
     */
    inline ByteBufferImpl::ByteBufferImpl(size_t size) : m_size(size) {
        if (size != 0)
            m_data = static_cast<u8*>(kmalloc(size));
        m_owned = true;
    }

    /**
     * @param data 
     * @param size 
     * @param mode 
     */
    inline ByteBufferImpl::ByteBufferImpl(const void* data, size_t size, ConstructionMode mode) : m_size(size) {
        ASSERT(mode == Copy);
        if (size != 0) {
            m_data = static_cast<u8*>(kmalloc(size));
            __builtin_memcpy(m_data, data, size);
        }
        m_owned = true;
    }

    /**
     * @param data 
     * @param size 
     * @param mode 
     */
    inline ByteBufferImpl::ByteBufferImpl(void* data, size_t size, ConstructionMode mode)
        : m_data(static_cast<u8*>(data))
        , m_size(size)
    {
        if (mode == Adopt) {
            m_owned = true;
        } else if (mode == Wrap) {
            m_owned = false;
        }
    }

    /**
     * @param size 
     */
    inline void ByteBufferImpl::grow(size_t size) {
        ASSERT(size > m_size);
        ASSERT(m_owned);
        if (size == 0) {
            if (m_data)
                kfree(m_data);
            m_data = nullptr;
            m_size = 0;
            return;
        }
        u8* new_data = static_cast<u8*>(kmalloc(size));
        __builtin_memcpy(new_data, m_data, m_size);
        u8* old_data = m_data;
        m_data = new_data;
        m_size = size;
        if (old_data)
            kfree(old_data);
    }

    /** 
     * @param size 
     * @return NonnullRefPtr<ByteBufferImpl> 
     */
    inline NonnullRefPtr<ByteBufferImpl> ByteBufferImpl::create_uninitialized(size_t size) {
        return ::adopt(*new ByteBufferImpl(size));
    }

    /**
     * @param size 
     * @return NonnullRefPtr<ByteBufferImpl> 
     */
    inline NonnullRefPtr<ByteBufferImpl> ByteBufferImpl::create_zeroed(size_t size) {
        auto buffer = ::adopt(*new ByteBufferImpl(size));
        if (size != 0)
            __builtin_memset(buffer->data(), 0, size);
        return buffer;
    }

    /**
     * @param data 
     * @param size 
     * @return NonnullRefPtr<ByteBufferImpl> 
     */
    inline NonnullRefPtr<ByteBufferImpl> ByteBufferImpl::copy(const void* data, size_t size) {
        return ::adopt(*new ByteBufferImpl(data, size, Copy));
    }

    /**
     * @param data 
     * @param size 
     * @return NonnullRefPtr<ByteBufferImpl> 
     */
    inline NonnullRefPtr<ByteBufferImpl> ByteBufferImpl::wrap(void* data, size_t size) {
        return ::adopt(*new ByteBufferImpl(data, size, Wrap));
    }

    /**
     * @brief adopt
     * 
     * @param data 
     * @param size 
     * @return NonnullRefPtr<ByteBufferImpl> 
     */
    inline NonnullRefPtr<ByteBufferImpl> ByteBufferImpl::adopt(void* data, size_t size) {
        return ::adopt(*new ByteBufferImpl(data, size, Adopt));
    }

    /**
     * @brief Streamer
     * 
     * @param stream 
     * @param value 
     * @return const LogStream& 
     */
    inline const LogStream& operator<<(const LogStream& stream, const ByteBuffer& value) {
        stream.write((const char*)value.data(), value.size());
        return stream;
    }

}

using Mods::ByteBuffer;
