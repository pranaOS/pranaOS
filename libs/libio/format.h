/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libio/formatter.h>
#include <libio/memoryreader.h>
#include <libio/memorywriter.h>
#include <libio/path.h>
#include <libio/scanner.h>
#include <libio/write.h>
#include <libutils/string.h>

namespace IO
{

static inline ResultOr<size_t> format(IO::Writer &writer, Scanner &scan)
{
    size_t written = 0;

    while (!scan.ended())
    {
        written += TRY(IO::write(writer, scan.next()));
    }

    return written;
}

template <typename First, typename... Args>
static inline ResultOr<size_t> format(Writer &writer, Scanner &scan, First first, Args... args)
{
    size_t written = 0;

    while (!scan.ended() && scan.peek() != '{')
    {
        written += TRY(IO::write(writer, scan.next()));
    }

    if (scan.peek() == '{')
    {
        auto fmt = Formatter::parse(scan);
        written += TRY(fmt.format(writer, first));
    }

    if (!scan.ended())
    {
        written += TRY(format(writer, scan, std::forward<Args>(args)...));
    }

    return written;
}

template <typename... Args>
static inline ResultOr<size_t> format(Writer &writer, const char *fmt, Args... args)
{
    MemoryReader memory{fmt};
    Scanner scan{memory};
    return format(writer, scan, std::forward<Args>(args)...);
}

static inline ResultOr<size_t> format(Writer &writer, const char *fmt)
{
    return IO::write(writer, fmt);
}

template <typename... Args>
static inline String format(const char *fmt, Args... args)
{
    MemoryWriter memory{};
    format(memory, fmt, std::forward<Args>(args)...);
    return memory.string();
}

} 