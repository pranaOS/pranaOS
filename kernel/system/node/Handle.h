/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <libabi/Handle.h>
#include <libio/Seek.h>
#include "system/node/Node.h"


struct FsHandle : public RefCounted<FsHandle>
{
private:
    Lock _lock{"fshandle"};
    RefPtr<FsNode> _node = nullptr;
    JOpenFlag _flags = 0;
    size_t _offset = 0;

public:
    void *attached;
    size_t attached_size;

    auto node() { return _node; }
    auto offset() { return _offset; }
    auto flags() { return _flags; }

    bool has_flag(JOpenFlag flag) { return (_flags & flag) == flag; }

    FsHandle(RefPtr<FsNode> node, JOpenFlag flags);

    FsHandle(FsHandle &other);

    ~FsHandle();
    


}