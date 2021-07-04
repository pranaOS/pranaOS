/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <string.h>
#include <libutils/Storage.h>
#include <libutils/Tags.h>

namespace Utils
{

struct StringStorage final :
    public Storage

{
private:
    char *_buffer;
    size_t _length;
}
}