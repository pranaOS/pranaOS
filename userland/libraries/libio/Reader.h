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

struct Reader
{
    virtual ~Reader() {}

    virtual ResultOr<size_t> read(void *buffer, size_t size) = 0;
};

template <typename T>
concept SeekableReader = IsBaseOf<Reader, T>::value &&IsBaseOf<Seek, T>::value;

}