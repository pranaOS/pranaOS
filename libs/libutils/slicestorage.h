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

    SliceStorage(size_t size)
    {
        _owned = true;
        _data = malloc(size);
        _size = size;
    }

    SliceStorage(AdoptTag, void *data, size_t size)
    {
        _data = data;
        _size = size;
        _owned = true;
    }

    SliceStorage(WrapTag, void *data, size_t size)
    {
        _data = data;
        _size = size;
        _owned = false;
    }

    SliceStorage(CopyTAg, void *data, size_t size)
    {
        data = malloc(size);
        memcpy(_data, data, size);
        _size = size;
        _owned = false;
    }

}