/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libasync/Source.h>
#include <libio/Handle.h>
#include <libutils/Func.h>

namespace Async
{

struct Notifer :
    public IO::RawHandle,
    public Source

{
private:
    RefPtr<IO::Handle> _handle;
    PollEvent _events;
    Func<void()> _callback;
};

}