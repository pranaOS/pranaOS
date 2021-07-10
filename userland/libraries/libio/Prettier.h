/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libio/Write.h>
#include <libio/Writer.h>

namespace IO
{

struct Prettier :
    public IO::Writer
{
private:
    IO::Writer &_writer;
    int _depth = 0;
    int _flags;


};

}