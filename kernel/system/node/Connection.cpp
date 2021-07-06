/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <string.h>
#include "system/node/Connection.h"
#include "system/node/Handle.h"

#define CONNECTION_BUFFER_SIZE 4000

FsConnection::FsConnection() : FsNode(J_FILE_TYPE_CONNECTION) {}

void FsConnection::accepted()
{
    _accepted = true;
}

bool FsConnection::is_accepted()
{
    return _accepted;
}

bool FsConnection::can_read(FsHandle &handle)
{
    if (handle.has_flag(J_OPEN_CLIENT))
    {
        return !_data_to_client.empty() || !server();
    }
    else
    {
        return !_data_to_server.empty() || !clients();
    }
}