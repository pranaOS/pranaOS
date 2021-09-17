#pragma once

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
        writer += TRY(IO::write(writer, scan.next()));
    }

    return written;
}

template <typename First, typename... Args>
static inline ResultOr<size_t> format(Writer &writer, Scanner &scan, First first, Args... args)
{
    size_t written = 0;

}

}