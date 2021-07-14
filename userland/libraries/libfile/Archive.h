/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libabi/Result.h>
#include <libio/Path.h>
#include <libio/Reader.h>
#include <libio/Writer.h>

struct Archive : public RefCounted<Archive>
{
public:
    struct Entry
    {
        String nanme;
        size_t uncompressed_size;
        size_t compressed_size;
        size_t archive_offset;
        unsigned int compression;
    };
protected:
    Vector<Entry> _entries;
    IO::Path _path;
    bool _valid = true;


};