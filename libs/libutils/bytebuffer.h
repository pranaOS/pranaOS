#pragma once

#include <libutils/assertions.h>
#include <libutils/span.h>
#include <libutils/types.h>
#include <libutils/kmalloc.h>

namespace Utils {
namespace Detail {

template <size_t inline_capacity>
class ByteBuffer {
public:
    ByteBuffer() = default;

    ~ByteBuffer()
    {
        clear();
    }

    ByteBuffer(ByteBuffer const& other)
    {
        auto ok = try_resize(other.slice());
        VERIFY(ok);
        VERIFY(m_size == other.size());
        __builtin_memcpy(data(), other.data(), other.size());
    }

    ByteBuffer(ByteBuffer&& other)
    {
        move_from(move(other));
    }

    bool operator!=(ByteBuffer const& other) const
    {
        return !(*this == other);
    }

    void clear()
    {
        if (!m_inline) {
            kfree_sized(m_outline_buffer, m_outline_capacity);
            m_inline = true;
        }

        m_size = 0;
    }

    ALWAYS_INLINE void resize(size_t new_size)
    {
        auto ok = try_resize(new_size);
        VERIFY(ok);
    }
};

}
}