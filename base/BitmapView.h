
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
    
};
    
}
