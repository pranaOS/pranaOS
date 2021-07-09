/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libutils/Array.h>
#include <libutils/Slice.h>
#include <libutils/Vector.h>
#include <libio/MemoryReader.h>
#include <libio/MemoryWriter.h>

namespace IO
{

constexpr int COPY_CHUNK_SIZE = 4096;

static inline JResult copy(Reader &from, Writer &to, size_t n)
{
    size_t remaining = n;

    do
    {
        Array<uint8_t, COPY_CHUNK_SIZE> copy_chunk;

        size_t read = TRY(from.read(copy_chunk.raw_storage(), MIN(COPY_CHUNK_SIZE, remaining)));

        if (read == 0)
        {
            to.flush();
            return SUCCESS;
        }

        size_t written = TRY(to.write(copy_chunk.raw_storage(), read));

        remaining -= read;

        if (written == 0 || remaining == 0)
        {
            to.flush();
            return SUCCESS;
        }
    } while (1);
}

static inline JResult copy(Reader &from, Writer &to)
{
    do
    {
        Array<uint8_t, COPY_CHUNK_SIZE> copy_chunk;

        auto read = TRY(from.read(copy_chunk.raw_storage(), copy_chunk.count()));

        if (read == 0)
        {
            to.flush();
            return SUCCESS;
        }

        auto written = TRY(to.write(copy_chunk.raw_storage(), read));

        if (written == 0)
        {
            to.flush();
            return SUCCESS;
        }
    } while (1);
}

static inline ResultOr<Slice> read_all(Reader &reader)
{
    MemoryWriter memory;

    TRY(copy(reader, memory));

    return Slice{memory.slice()};
}

static inline ResultOr<Slice> read_all(MemoryReader &reader)
{
    if (TRY(reader.tell()) == 0)
    {
        return reader.memory();
    }
    else
    {
        return read_all((Reader &)reader);
    }
}

    
}
