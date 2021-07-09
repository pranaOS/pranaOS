/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <pranaos/Time.h>
#include <libasync/Source.h>
#include <libutils/Func.h>
#include <libutils/RefPtr.h>

namespace Async
{


struct Timer :
    public Source
{
private:
    bool _running = false;
    TimeStamp _scheduled = 0;
    Timeout _interval = 0;
    Func<void()> _callback;



};

}