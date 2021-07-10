/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libabi/Handle.h>
#include <libutils/ResultOr.h>

namespace IO
{

enum struct Whence : uint8_t
{
    START = J_WHENCE_START,
    CURRENT = J_WHENCE_CURRENT,
    END = J_WHENCE_END,
};

struct SeekFrom
{

};

}