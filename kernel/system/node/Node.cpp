/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <string.h>
#include "system/node/Handle.h"
#include "system/node/Node.h"

FsNode::FsNode(HjFileType type)
{
    _type = type;
}

void FsNode::ref_handle(FsHandle &handle)
{
    if (handle.flags() & HJ_OPEN_READ)
    {
        __atomic_add_fetch(&_readers, 1, __ATOMIC_SEQ_CST);
    }

    if (handle.flags() & HJ_OPEN_WRITE)
    {
        __atomic_add_fetch(&_writers, 1, __ATOMIC_SEQ_CST);
    }

    if (handle.flags() & HJ_OPEN_CLIENT)
    {
        __atomic_add_fetch(&_clients, 1, __ATOMIC_SEQ_CST);
    }

    if (handle.flags() & HJ_OPEN_SERVER)
    {
        __atomic_add_fetch(&_server, 1, __ATOMIC_SEQ_CST);
    }
}

void FsNode::deref_handle(FsHandle &handle)
{
    if (handle.flags() & HJ_OPEN_READ)
    {
        __atomic_sub_fetch(&_readers, 1, __ATOMIC_SEQ_CST);
    }

    if (handle.flags() & HJ_OPEN_WRITE)
    {
        __atomic_sub_fetch(&_writers, 1, __ATOMIC_SEQ_CST);
    }

    if (handle.flags() & HJ_OPEN_CLIENT)
    {
        __atomic_sub_fetch(&_clients, 1, __ATOMIC_SEQ_CST);
    }

    if (handle.flags() & HJ_OPEN_SERVER)
    {
        __atomic_sub_fetch(&_server, 1, __ATOMIC_SEQ_CST);
    }
}
