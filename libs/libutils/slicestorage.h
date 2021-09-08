/*
* Copyright (c) 2021, Krisna Pranav
*
* SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <string.h>
#include <libutils/storage.h>
#include <libutils/tags.h>

struct SliceStorage final : public Storage
{
private:
    void *_data = nullptr;
    size_t _size = 0;
    bool _owned = false;

public:
    using Storage::end;
    using Storage::start;

    void *start() override
    {
        return _data;
    }

    void *end() override 
    { 
        return reinterpret_cast<char *>(start()) + _size; 
    }

}