/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libio/Path.h>
#include "system/node/Handle.h"
#include "system/node/Node.h"


struct Domain
{
private:
    RefPtr<FsNode> _root;

public:
    RefPtr<FsNode> root() { return _root; }
}