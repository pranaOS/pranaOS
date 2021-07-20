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

};

}
}
