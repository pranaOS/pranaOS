/*
 * Copyright (c) 2021, nuke123-sudo
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include "system/node/Node.h"

struct FsProcessInfo : public FsNode
{
private:
public:
    FsProcessInfo();

    JResult open(FsHandle &handle) override;

    void close(FsHandle &handle) override;

    ResultOr<size_t> read(FsHandle &handle, void *buffer, size_t size) override;
};

void process_info_initialize();
