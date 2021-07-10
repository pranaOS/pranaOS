/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libio/Reader.h>

namespace IO
{

struct ReadCounter :
    public Reader
{
private:
    size64_t _count = 0;
    Reader &_reader;
};

}