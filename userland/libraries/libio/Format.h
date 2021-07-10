/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libio/Formatter.h>
#include <libio/MemoryReader.h>
#include <libio/MemoryWriter.h>
#include <libio/Path.h>
#include <libio/Scanner.h>
#include <libio/Write.h>
#include <libutils/String.h>

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



}