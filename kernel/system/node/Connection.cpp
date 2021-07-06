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