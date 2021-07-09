/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

// incldues
#include <libasync/Loop.h>
#include <libutils/RefPtr.h>

namespace Async
{

struct Source
{
private:
    RefPtr<Loop> _eventloop;

public:
    Loop &loop() { return *_eventloop; }

    Source()
    {
        _eventloop = Loop::the();
    }

    virtual ~Source()
    {
    }
};

}