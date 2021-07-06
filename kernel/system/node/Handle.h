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
}