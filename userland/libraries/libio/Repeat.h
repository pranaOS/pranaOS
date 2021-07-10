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

struct Repeat :
    public Reader
{
private:
    uint8_t c;

};

}