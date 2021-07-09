/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libio/Path.h>
#include <libio/Handle.h>
#include <libio/Reader.h>
#include <libio/Writer.h>

namespace IO
{

struct File final :
    public Reader,
    public Writer,
    public Seek,
    public RawHandle
{
private:
    RefPtr<Handle> _handle;
    Optional<IO::Path> _path = NONE;

public:
    const Optional<IO::Path> &path() { return _path; }

    File() {}

    File(const char *path, JOpenFlags flags = 0);

    File(String path, JOpenFlag flags = 0);

    File(IO::Path &path, JOpenFlag flags = 0);

    File(RefPtr<Handle> handle);
    

};

}