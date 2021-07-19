/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <base/BitmapView.h>
#include <base/Noncopyable.h>
#include <base/Optional.h>
#include <base/Platform.h>
#include <base/StdLibExtras.h>
#include <base/Types.h>
#include <base/kmalloc.h>

namespace Base {

class Bitmap {
    Base_MAKE_NONCOPYABLE(Bitmap);

public:
    Bitmap() = default;

    Bitmap(size_t size, bool default_value)
        : m_size(size)
    {
        VERIFY(m_size != 0);
        m_data = static_cast<u8*>(kmalloc(size_in_bytes()));
        fill(default_value);
    }

    Bitmap(u8* data, size_t size, bool is_owning = false)
        : m_data(data)
        , m_size(size)
        , m_is_owning(is_owning)
    {
    }


};

}