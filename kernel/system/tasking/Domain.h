/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libio/Path.h>
#include "system/node/Handle.h"
#include "system/node/Node.h"

struct Domain
{
private:
    RefPtr<FsNode> _root;

public:
    RefPtr<FsNode> root() { return _root; }

    Domain();

    Domain(const Domain &other);

    ~Domain();

    Domain &operator=(const Domain &other);

    RefPtr<FsNode> find(IO::Path path);

    ResultOr<RefPtr<FsHandle>> open(IO::Path path, HjOpenFlag flags);

    ResultOr<RefPtr<FsHandle>> connect(IO::Path path);

    JResult link(IO::Path path, RefPtr<FsNode> node);

    JResult unlink(IO::Path path);

    JResult rename(IO::Path old_path, IO::Path new_path);

    JResult mkdir(IO::Path path);

    JResult mkpipe(IO::Path path);

    JResult mklink(IO::Path old_path, IO::Path new_path);
};