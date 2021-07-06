/*
 * Copyright (c) 2021, krishpranav, Alex5xt
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

// includes
#include <libutils/RingBuffer.h>
#include "system/node/Node.h"

struct FsPipe : public FsNode
{
private:
    static constexpr int BUFFER_SIZE = 4000;
    RingBuffer<char> _buffer{BUFFER_SIZE};

public:
    FsPipe();

    bool can_read(FsHandle &handle) override;
}