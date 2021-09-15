/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <libutils/refptr.h>

namespace Utils
{

struct Storage : public RefCounted<Storage>
{
    virtual ~Storage() {}

    virtual void *end() = 0;
    virtual void *start() = 0;
};

struct RawStorage
{
    virtual ~RawStorage() {}

    virtual RefPtr<Storage> storage() = 0;
};

}