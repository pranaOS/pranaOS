/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "system/Streams.h"
#include "system/node/Directory.h"
#include "system/node/File.h"
#include "system/node/Pipe.h"
#include "system/node/Socket.h"
#include "system/scheduling/Scheduler.h"
#include "system/tasking/Domain.h"

Domain::Domain()
{
    _root = make<FsDirectory>();
}

Domain::Domain(const Domain &parent)
{
    _root = parent._root;
}

Domain::~Domain()
{
}

Domain &Domain::operator=(const Domain &other)
{
    if (this != &other)
    {
        _root = other._root;
    }

    return *this;
}

RefPtr<FsNode> Domain::find(IO::Path path)
{
    auto current = root();

    for (size_t i = 0; i < path.length(); i++)
    {
        if (current && current->type() == J_FILE_TYPE_DIRECTORY)
        {
            auto element = path[i];

            current->acquire(scheduler_running_id());
            auto found = current->find(element);
            current->release(scheduler_running_id());

            current = found;
        }
        else
        {
            return nullptr;
        }
    }

    return current;
}