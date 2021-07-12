/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "system/scheduling/Blocker.h"
#include "system/tasking/Task.h"

bool BlockerAccept::can_unblock(Task &)
{
    return !_node->is_acquire() && _node->can_accept();
}


void Blocker::on_unblock(Task &task)
{
    _node->acquire(task.id);
}

bool BlockerConnect::can_unblock(Task &)
{
    return _connection->is_accpeted();
}

bool BlockerRead::can_unblock(Task &)
{
    return !_handle.node()->is_acquire() && _handle.node()->can_read(_handle);
}

void BlockerRead::on_unblock(Task &task)
{
    _handle.node()->acquire(task.id);
}

bool BlockerSelect::can_unblock(Task &)
{
    bool should_be_unblock = false;

    for (size_t i = 0; i < _handles.count(); i++)
    {
        auto &selected = _handles[i];

        selected.result = selected.handle->poll(selected.events);

        if (selected.result != 0)
        {
            should_be_unblock = true;
        }
    }

    return should_be_unblock;
}
