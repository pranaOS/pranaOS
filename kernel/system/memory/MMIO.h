/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libmath/MinMax.h>
#include <libutils/Prelude.h>
#include <libutils/RefCounted.h>
#include <string.h>

#include "system/memory/MemoryRange.h"

struct MMIORange : public RefCounted<MMIORange>
{
private:
    MemoryRange _virtual_range = {};
    bool _own_physical_range = false;
    MemoryRange _physical_range = {};

public:
    uintptr_t base() { return _virtual_range.base(); }
};