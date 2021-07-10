/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libio/Reader.h>
#include <libmath/MinMax.h>

namespace IO
{

struct ScopedReader :
    public Reader
{
private:
    Reader &_reader;
    size_t _count = 0;
    size_t _lenght = 0;

public:
    

};

}