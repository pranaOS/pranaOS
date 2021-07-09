/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libio/Handle.h>
#include <libio/Path.h>
#include <libio/Reader.h>
#include <libio/Writer.h>

namespace IO
{

struct Connection final :
    public Reader,
    public Writer,
    public RawHandle
{
private:
    RefPtr<Handle> _handle;


}

}