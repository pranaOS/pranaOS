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

public:
    const Optional<IO::Path> &path() { return _path; }
    const Vector<Entry> &entries() { return _entries; }

    Directory() {}
    Directory(const char *path);
    Directory(String path);
    Directory(const IO::Path &path);
    Directory(RefPtr<Handle> handle);

    RefPtr<Handle> handle() override { return _handle; }

    bool exist();
};

} 