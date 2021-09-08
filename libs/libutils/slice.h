/*
* Copyright (c) 2021, Krisna Pranav
*
* SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libutils/prelude.h>
#include <libutils/refptr.h>
#include <libutils/sliceStorage.h>
#include <libutils/std.h>

struct Slice : public RawStorage
{
private:
    RefPtr<Storage> _storage;

    const void *_start = nullptr;
    size_t _size = 0;


}