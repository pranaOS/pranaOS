
/*
 * Copyright (c) 2021, Alex5xt
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Array.h>
#include <base/Optional.h>
#include <base/Platform.h>
#include <base/StdLibExtras.h>
#include <base/Types.h>

namespace Base {

static constexpr Array bitmask_first_byte = { 0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80 };
static constexpr Array bitmask_last_byte = { 0x00, 0x1, 0x3, 0x7, 0xF, 0x1F, 0x3F, 0x7F };

class BitmapView {
public:
    BitmapView(u8* data, size_t size)
        : m_data(data)
        , m_size(size)
    {
    }

    size_t size() const { return m_size; }
    size_t size_in_bytes() const { return ceil_div(m_size, static_cast<size_t>(8)); }
    bool get(size_t index) const
    {
        VERIFY(index < m_size);
        return 0 != (m_data[index / 8] & (1u << (index % 8)));
    }

    void set(size_t index, bool value) const
    {
        VERIFY(index < m_size);
        if (value)
            m_data[index / 8] |= static_cast<u8>((1u << (index % 8)));
        else
            m_data[index / 8] &= static_cast<u8>(~(1u << (index % 8)));
    }

    size_t count_slow(bool value) const
    {
        return count_in_range(0, m_size, value);
    }

        size_t count_in_range(size_t start, size_t len, bool value) const
    {
        VERIFY(start < m_size);
        VERIFY(start + len <= m_size);
        if (len == 0)
            return 0;

        size_t count;
        const u8* first = &m_data[start / 8];
        const u8* last = &m_data[(start + len) / 8];
        u8 byte = *first;
        byte &= bitmask_first_byte[start % 8];
        if (first == last) {
            byte &= bitmask_last_byte[(start + len) % 8];
            count = __builtin_popcount(byte);
        } else {
            count = __builtin_popcount(byte);
            if (last < &m_data[size_in_bytes()]) {
                byte = *last;
                byte &= bitmask_last_byte[(start + len) % 8];
                count += __builtin_popcount(byte);
            }
            if (++first < last) {
                const u32* ptr32 = (const u32*)(((FlatPtr)first + sizeof(u32) - 1) & ~(sizeof(u32) - 1));
                if ((const u8*)ptr32 > last)
                    ptr32 = (const u32*)last;
                while (first < (const u8*)ptr32) {
                    count += __builtin_popcount(*first);
                    first++;
                }
                const u32* last32 = (const u32*)((FlatPtr)last & ~(sizeof(u32) - 1));
                while (ptr32 < last32) {
                    count += __builtin_popcountl(*ptr32);
                    ptr32++;
                }
                for (first = (const u8*)ptr32; first < last; first++)
                    count += __builtin_popcount(*first);
            }
        }

        if (!value)
            count = len - count;
        return count;
    }

    bool is_null() const { return !m_data; }

    const u8* data() const { return m_data; }

        template<bool VALUE>
    Optional<size_t> find_one_anywhere(size_t hint = 0) const
    {
        VERIFY(hint < m_size);
        const u8* end = &m_data[m_size / 8];

        for (;;) {
            const u32* ptr32 = (const u32*)((FlatPtr)&m_data[hint / 8] & ~(sizeof(u32) - 1));
            if ((const u8*)ptr32 < &m_data[0]) {
                ptr32++;

                size_t start_ptr32 = (const u8*)ptr32 - &m_data[0];
                size_t i = 0;
                u8 byte = VALUE ? 0x00 : 0xff;
                while (i < start_ptr32 && m_data[i] == byte)
                    i++;
                if (i < start_ptr32) {
                    byte = m_data[i];
                    if constexpr (!VALUE)
                        byte = ~byte;
                    VERIFY(byte != 0);
                    return i * 8 + __builtin_ffs(byte) - 1;
                }
            }

            u32 val32 = VALUE ? 0x0 : 0xffffffff;
            const u32* end32 = (const u32*)((FlatPtr)end & ~(sizeof(u32) - 1));
            while (ptr32 < end32 && *ptr32 == val32)
                ptr32++;

            if (ptr32 == end32) {

                u8 byte = VALUE ? 0x00 : 0xff;
                size_t i = (const u8*)ptr32 - &m_data[0];
                size_t byte_count = m_size / 8;
                VERIFY(i <= byte_count);
                while (i < byte_count && m_data[i] == byte)
                    i++;
                if (i == byte_count) {
                    if (hint <= 8)
                        return {}; 

                    end = (const u8*)((FlatPtr)&m_data[hint / 8] & ~(sizeof(u32) - 1));
                    hint = 0;
                    continue;
                }
                byte = m_data[i];
                if constexpr (!VALUE)
                    byte = ~byte;
                VERIFY(byte != 0);
                return i * 8 + __builtin_ffs(byte) - 1;
            }

            val32 = *ptr32;
            if constexpr (!VALUE)
                val32 = ~val32;
            VERIFY(val32 != 0);
            return ((const u8*)ptr32 - &m_data[0]) * 8 + __builtin_ffsl(val32) - 1;
        }
    }
    
};
    
}


using Base::BitmapView;