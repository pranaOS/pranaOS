/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libutils/RingBuffer.h>
#include "system/node/Node.h"

struct FsTerminal : public FsNode
{
private:
    static constexpr int BUFFER_SIZE = 1024;

    int _width = 80;
    int _height = 25;

};