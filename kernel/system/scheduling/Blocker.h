/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <skift/Time.h>

#include <libutils/Vector.h>

#include "system/node/Handle.h"
#include "system/system/System.h"

struct Task;

struct Blocker
{
private:
    HjResult _result = SUCCESS;
    TimeStamp _timeout = -1;
    bool _interrupted = false;

public:
    HjResult result() { return _result; }

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

    void interrupt(Task &task, HjResult result)
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

    virtual bool can_unblock(Task &) { return true; }

    virtual void on_unblock(Task &) {}

    virtual void on_timeout(Task &) {}

    virtual void on_interrupt(Task &) {}
};

struct BlockerAccept : public Blocker
{
private:
    RefPtr<FsNode> _node;

public:
    BlockerAccept(RefPtr<FsNode> node) : _node(node)
    {
    }

    bool can_unblock(Task &task) override;

    void on_unblock(Task &task) override;
};

struct BlockerConnect : public Blocker
{
private:
    RefPtr<FsNode> _connection;

public:
    BlockerConnect(RefPtr<FsNode> connection)
            : _connection(connection)
    {
    }

    bool can_unblock(Task &task) override;
};
