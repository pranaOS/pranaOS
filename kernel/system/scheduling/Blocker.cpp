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


