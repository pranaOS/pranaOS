/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libio/Copy.h>
#include <libio/Reader.h>
#include <libutils/Assert.h>
#include <libutils/Vector.h>

namespace IO
{

template <typename T>
ResultOr<size_t> read_vector(Reader &reader, Vector<T> &vector)
{
    size_t read = 0;

    T object;
    size_t last_read = TRY(reader.read(&object, sizeof(T)));
    while (last_read > 0)
    {
        vector.push_back(std::move(object));
        read += last_read;
        
        last_read = TRY(reader.read(&object, sizeof(T)));
    }

    return read;

}

inline ResultOr<String> read_string(Reader &reader, size_t len)
{
    IO::MemoryWriter memory;
    TRY(IO::copy(reader, memory, len));
    return String{memory.string()};
}

template <typename T>
inline ResultOr<T> peek(SeekableReader auto &reader)
{
    auto result = TRY(read<T>(reader));
    reader.seek(IO::SeekFrom::current(-sizeof(T)));
    return result;
}

}