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

ResultOr<RefPtr<FsHandle>> Domain::open(IO::Path path, JOpenFlag flags)
{
    bool should_create_if_not_present = (flags & J_OPEN_CREATE) == J_OPEN_CREATE;

    auto node = find(path);

    if (!node && should_create_if_not_present)
    {
        auto parent = find(path.dirpath());

        if (parent)
        {
            if (flags & J_OPEN_SOCKET)
            {
                node = make<FsSocket>();
            }
            else
            {
                node = make<FsFile>();
            }

            parent->acquire(scheduler_running_id());
            parent->link(path.basename(), node);
            parent->release(scheduler_running_id());
        }
    }

    if (!node)
    {
        return ERR_NO_SUCH_FILE_OR_DIRECTORY;
    }

    if ((flags & J_OPEN_DIRECTORY) && node->type() != J_FILE_TYPE_DIRECTORY)
    {
        return ERR_NOT_A_DIRECTORY;
    }

    if ((flags & J_OPEN_SOCKET) && node->type() != J_FILE_TYPE_SOCKET)
    {
        return ERR_NOT_A_SOCKET;
    }

    bool is_node_stream = node->type() == J_FILE_TYPE_PIPE ||
                          node->type() == J_FILE_TYPE_REGULAR ||
                          node->type() == J_FILE_TYPE_DEVICE ||
                          node->type() == J_FILE_TYPE_TERMINAL;

    if ((flags & J_OPEN_STREAM) && !(is_node_stream))
    {
        return ERR_NOT_A_STREAM;
    }

    return make<FsHandle>(node, flags);
}

ResultOr<RefPtr<FsHandle>> Domain::connect(IO::Path path)
{
    auto node = find(path);

    if (!node)
    {
        return ERR_NO_SUCH_FILE_OR_DIRECTORY;
    }

    if (node->type() != J_FILE_TYPE_SOCKET)
    {
        return ERR_SOCKET_OPERATION_ON_NON_SOCKET;
    }

    node->acquire(scheduler_running_id());
    auto connection_or_result = node->connect();
    node->release(scheduler_running_id());

    if (!connection_or_result.success())
    {
        return connection_or_result.result();
    }

    auto connection = connection_or_result.unwrap();
    auto connection_handle = make<FsHandle>(connection, J_OPEN_CLIENT);

    BlockerConnect blocker{connection};
    TRY(task_block(scheduler_running(), blocker, -1));

    return connection_handle;
}

JResult Domain::mkdir(IO::Path path)
{
    if (path.length() == 0)
    {
        // We are trying to create /
        return ERR_FILE_EXISTS;
    }

    return link(path, make<FsDirectory>());
}

JResult Domain::mkpipe(IO::Path path)
{
    return link(path, make<FsPipe>());
}