/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <string.h>
#include "system/node/Handle.h"
#include "system/node/Node.h"


FsNode::FsNode(JFileType type)
{
    _type = type;
}   

void FsNode::ref_handle(FsHandle &handle)
{
    if (handle.flags() & J_OPEN_READ)
    {
        __atomic_add_fetch(&_readers, 1, __ATOMIC_SEQ_CST);
    }
}