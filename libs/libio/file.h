/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <libio/path.h>

namespace IO
{

struct File final : public Reader, public Writer, public Seek, public RawHandle
{
private:
    RefPtr<Handle> _handle;
    Optional<IO::Path> _path = NONE;

public:
    const Optional<IO::Path> &path() 
    {
        return _path;
    }

    File() {}
};

}