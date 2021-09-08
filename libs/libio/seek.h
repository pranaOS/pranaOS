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
    CURRENT = J_WHENCE_CURRENT,
    END = J_WHENCE_END,
}

struct SeekFrom
{
    Whence whence;
    ssize64_t position;
        
    static SeekFrom start(ssize64_t position = 0)
    {
        return {position};
    }

};
