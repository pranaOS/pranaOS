/*
 * Copyright (c) 2021, evilbat831
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include "system/node/Node.h"

struct FsFile : public FsNode
{
private:
    char *_buffer;
    size_t _buffer_allocated;
    size_t _buffer_size;


}