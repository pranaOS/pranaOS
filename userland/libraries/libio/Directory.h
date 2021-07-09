/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libio/Handle.h>
#include <libio/Path.h>

namespace IO
{

struct Directory :
    public RawHandle

{
public:
    struct Entry
    {
        String name;
        JStat stat;
    };

private:
    RefPtr<Handle> _handle;
    Optional<IO::Path> _path = NONE;
    Vector<Entry> _entries;

    JResult read_entries();

};

}