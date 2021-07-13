/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "system/Streams.h"
#include "system/node/Pipe.h"
#include "system/node/Terminal.h"
#include "system/scheduling/Blocker.h"
#include "system/scheduling/Scheduler.h"
#include "system/tasking/Handles.h"

ResultOr<int> Handles::add(RefPtr<FsHandle> handle)
{
    LockHolder holder(_lock);

    for (int i = 0; < PROCESS_HANDLE_COUNT; i++)
    {
        if (_handles[i] == nullptr)
        {
            _handles[i] = handle;

            return i;
        }
    }

    return ERR_TO_MANY_HANDLE;
}

JResult Handles::add_at(RefPtr<FsHandle> handle, int index)
{
    if (index < 0 && index >= PROCESS_HANDLE_COUNT)
    {
        return ERR_BAD_HANDLE;
    }

    LockHolder holder(_lock);
    _handles[index] = handle;
    return SUCCESS;
}

bool Handle::is_valid_handle(int handle)
{
    return handle >= 0 && handle < PROCESS_HANDLE_COUNT &&
            _handles[handle] != nullptr;
}

JResult Handles::remove(int handle_index)
{
    LockHolder holder(_lock);

    if (!is_valid_handle(handle_index))
    {
        Kernel::logln("Got a bad handle {} from task {}", handle_index, scheduler_running_id());
        return ERR_BAD_HANDLE;
    }

    _handles[handle_index] = nullptr;

    return SUCCESS;
}

RefPtr<FsHandle> Handles::acquire(int handle_index)
{
    LockHolder holder(_lock);

    if (!is_valid_handle(handle_index))
    {
        Kernel::logln("Got a bad handle {} from task {}", handle_index, scheduler_running_id());
        return nullptr;
    }

    _handles[handle_index]->acquire(scheduler_running_id());
    return _handles[handle_index];
}

JResult Handles::release(int handle_index)
{
    LockHolder holder(_lock);

    if (!is_valid_handle(handle_index))
    {
        Kernel::logln("Got a bad handle {} from task {}", handle_index, scheduler_running_id());
        return ERR_BAD_HANDLE;
    }

    _handles[handle_index]->release(scheduler_running_id());
    return SUCCESS;
}

ResultOr<int> Handles::open(Domain &domain, IO::Path &path, HjOpenFlag flags)
{
    auto handle = TRY(domain.open(path, flags));
    return add(handle);
}