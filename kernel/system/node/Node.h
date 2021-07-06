/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <libabi/Filesystem.h>
#include <libabi/IOCall.h>
#include <libabi/Result.h>
#include <libutils/Lock.h>
#include <libutils/RefPtr.h>
#include <libutils/ResultOr.h>
#include <libutils/String.h>

struct FsNode;
struct FsHandle;

struct FsNode : public RefCounted<FsNode>
{
private:
    Lock _lock{"fsnode"};
    JFileType _type;

    unsigned int _readers = 0;
    unsigned int _writers = 0;
    unsigned int _clients = 0;
    unsigned int _server = 0;

    
}