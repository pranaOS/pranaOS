/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libio/Seek.h>

namespace IO
{

struct Writer
{
    virtual ~Writer() {}

    virtual ResultOr<size_t> write(const void *buffer, size_t size) = 0;

    virtual JResult flush() { return SUCCESS; }
};

template <typename T>
concept SeekableWriter = IsBaseOf<Writer, T>::value &&IsBaseOf<Seek, T>::value;

}