/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libutils/RingBuffer.h>
#include "system/node/Node.h"

struct FsConnection : public FsNode
{
private:
    static constexpr int BUFFER_SIZE = 4000;

    bool _accepted = false;

    RingBuffer<char> _data_to_server{BUFFER_SIZE};
    RingBuffer<char> _data_to_client{BUFFER_SIZE};


}