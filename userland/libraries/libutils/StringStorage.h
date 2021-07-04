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

public:
    using Storage::end;
    using Storage::start;

    const char *cstring() { return _buffer; }

    void *start() override { return _buffer; }

    void *end() override { return reinterpret_cast<char *>(start()) + _length; }


};
}