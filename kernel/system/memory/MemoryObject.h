/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libutils/Prelude.h>

struct MemoryObject
{
    int id;
    MemoryRange _range;

    int refcount;

    auto range() { return _range; }
};

void memory_object_initialize();

MemoryObject *memory_object_create(size_t size);