/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <libfile/Archive.h>
#include <libfile/TARArchive.h>
#include <libfile/ZipArchive.h>

RefPtr<Archive> Archive::open(IO::Path path, bool read)
{
    if (path.extension() == ".zip")
    {
        return make<ZipArchive>(path, read);
    }
    else if (path.extension() == ".tar")
    {
        return make<TARArchive>(path, read);
    }
    else
    {
        return nullptr;
    }
}