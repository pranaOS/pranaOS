/*
 * Copyright (c) 2021, evilbat831
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <libutils/Vector.h>

#include "system/node/Node.h"

struct FileListing
{
    size_t count;
    JDirEntry entries[];
};

struct FsJDirEntry
{
    String name;
    RefPtr<FsNode> node;
};

struct FsDirectory : public FsNode
{
private:
    Vector<FsJDirEntry> _children{};

public:
    FsDirectory();

    JResult open(FsHandle &handle) override;

    void close(FsHandle &handle) override;

    ResultOr<size_t> read(FsHandle &handle, void *buffer, size_t size) override;

    RefPtr<FsNode> find(String name) override;

    JResult link(String name, RefPtr<FsNode> child) override;

    JResult unlink(String name) override;
};
