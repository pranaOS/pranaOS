/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <libio/File.h>

namespace IO
{

File::File(const char *path, HjOpenFlag flags)
    : _handle{make<Handle>(path, flags | HJ_OPEN_STREAM)},
      _path{IO::Path::parse(path)}
{
}

File::File(String path, HjOpenFlag flags)
    : _handle{make<Handle>(path, flags | HJ_OPEN_STREAM)},
      _path{IO::Path::parse(path)}
{
}

File::File(IO::Path &path, HjOpenFlag flags)
    : _handle{make<Handle>(path.string(), flags | HJ_OPEN_STREAM)},
      _path{path}
{
}

File::File(RefPtr<Handle> handle)
    : _handle{handle}
{
}
    
}
