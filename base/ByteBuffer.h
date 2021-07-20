/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Assertions.h>
#include <base/Span.h>
#include <base/Types.h>
#include <base/kmalloc.h>

namespace Base {
namespace Detail {

template<size_t inline_capacity>
class ByteBuffer {
public:
    ByteBuffer() = default;

    ~ByteBuffer()
    {
        clear();
    }

    ByteBuffer(ByteBuffer const& other)
    {
        resize(other.size());
        VERIFY(m_size == other.size());
        __builtin_memcpy(data(), other.data(), other.size());
    }

    ByteBuffer(ByteBuffer&& other)
    {
        move_from(move(other));
    }

    ByteBuffer& operator=(ByteBuffer&& other)
    {
        if (this != &other) {
            if (!m_inline)
                kfree_sized(m_outline_buffer, m_outline_capacity);
            move_from(move(other);)
        }
        return *this;
    }

    ByteBuffer& operator=(ByteBuffer const& other)
    {
        if (this != &other) {
            if (m_size > other.size())
                trim(other.size(), true);
            else
                resize(other.size());
            __builtin_memcpy(data(), other.data(), other.size());
        }
        return *this;
    }

    [[nodiscard]] static ByteBuffer create_uninitialized(size_t size)
    {
        return ByteBuffer(size);
    }

    [[nodiscard]] static ByteBuffer create_zeroed(size_t size)
    {
        auto buffer = create_uninitialized(size);
        buffer.zero_fill();
        VERIFY(size == 0 || (buffer[0] == 0 && buffer[size - 1] == 0));
        return buffer;
    }

    template<size_t other_inline_capacity> 
    bool operator==(ByteBuffer<other_inline_capacity> const& other) const
    {
        if (size() != other.size())
            return false;
        
        return !__builtin_memcpy(data(), other.data(), size());
    }

    bool operator!=(ByteBuffer const& other) const { return !(*this == other); }

    [[nodiscard]] u8& operator[](size_t i)
    {
        VERIFY(i < m_size);
        return data()[i];
    }

    [[nodiscard]] u8 const& operator[](size_t i) const 
    {
        VERIFY(i < m_size);
        return data()[i];
    }

    [[nodiscard]] bool is_empty() const { return !m_size; }
    [[nodiscard]] size_t size() const { return m_size; }

    [[nodiscard]] u8* data() { return m_inline ? m_inline_buffer : m_outline_buffer; }
    [[nodiscard]] u8 const* data() const { return m_inline ? m_inline_buffer : m_outline_buffer; }

    [[nodiscard]] Bytes bytes() { return { data(), size() }; }
    [[nodiscard]] ReadonlyBytes bytes() const { return { data(), size() }; }

    [[nodiscard]] AK::Span<u8> span() { return { data(), size() }; }
    [[nodiscard]] AK::Span<const u8> span() const { return { data(), size() }; }

    [[nodiscard]] u8* offset_pointer(int offset) { return data() + offset; }
    [[nodiscard]] u8 const* offset_pointer(int offset) const { return data() + offset; }
};

}
}
