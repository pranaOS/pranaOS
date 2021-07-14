/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libio/Streams.h>
#include "archs/Arch.h"
#include "system/graphics/EarlyConsole.h"
#include "system/interrupts/Interupts.h"
#include "system/scheduling/Scheduler.h"

namespace Kernel
{

struct LogStream : public IO::Writer
{
    ResultOr<size_t> write(const void *buffer, size_t size) override
    {
        early_console_write(buffer, size);
        return Arch::debug_write(buffer, size);
    }
};;

}