/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <pranaos/Time.h>
#include <libutils/Vector.h>
#include "system/node/Handle.h"
#include "system/system/System.h"

struct Task;

struct Blocker
{
private:
    JResult _result = SUCCESS;
    TimeStamp _timeout = -1;
    bool _interrupted = false;

public:
    JResult result() { return _result; }

    void timeout(TimeStamp ts) { _timeout = ts; }

    virtual ~Blocker() {}

    void unblock(Task &task)
    {
        _result = SUCCESS;
        on_unblock(task);
    }

    void timeout(Task &task)
    {
        _result = TIMEOUT;
        on_timeout(task);
    }

    void interrupt(Task &task, JResult result)
    {
        _interrupted = true;
        _result = result;
        on_interrupt(task);
    }

    bool has_timeout()
    {
        return _timeout != (Timeout)-1 && _timeout <= system_get_tick();
    }

    bool is_interrupted()
    {
        return _interrupted;
    }

};