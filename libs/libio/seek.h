/*
* Copyright (c) 2021, Krisna Pranav
*
* SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <libabi/handle.h>
#include <libutils/resultor.h>

namespace IO
{

enum struct Whence : uint8_t
{
    START = J_WHENCE_START,
    END = J_WHENCE_END,
}

}