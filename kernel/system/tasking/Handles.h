/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libio/Path.h>
#include "system/node/Handle.h"

struct Handles
{
private:
    Lock _lock{"handles-lock"};

    RefPtr<FsHandle> _handles[PROCESS_HANDLE_COUNT];

    ResultOr<int> add(RefPtr<FsHandle> handle);

    JResult add_at(RefPtr<FsHandle> handle, int index);

    bool is_valid_handle(int handle);

    JResult remove(int handle_index);

    RefPtr<FsHandle> acquire(int handle_index);

    JResult release(int handle_index);

};