/*
 * Copyright (c) 2021, nuke123-sudo
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libio/Reader.h>
#include <libmath/MinMax.h>
#include <libutils/Slice.h>

namespace IO
{

struct MemoryReader final :
    public Reader,
    public Seek
{

private:
    Slice _memory;
    size_t _position = 0;


};

}