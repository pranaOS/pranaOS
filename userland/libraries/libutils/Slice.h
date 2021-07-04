/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libutils/Prelude.h>
#include <libutils/RefPtr.h>
#include <libutils/SliceStorage.h>
#include <libutils/Std.h>

struct Slice :
    public RawStorage
{
private:
    RefPtr<Storage> _storage;

    const void *_start = nullptr;
    size_t size = 0;


}