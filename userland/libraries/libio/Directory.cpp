/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <libio/Directory.h>

namespace IO
{

JResult Directory::read_entries()
{
    JDirEntry entry;

    auto read = TRY(_handle->read(&entry, sizeof(entry)));

    while (read > 0)
    {
        _entries.push_back({entry.name, entry.stat});
        read = TRY(_handle->read(&entry, sizeof(entry)));
    }

    _entries.sort([](auto &left, auto &right) {
        return strcmp(left.name.cstring(), right.name.cstring());
    });

    return SUCCESS;
}

}