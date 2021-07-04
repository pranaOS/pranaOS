/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libutils/Hash.h>
#include <libutils/RefPtr.h>
#include <libutils/Slice.h>
#include <libutils/Std.h>
#include <libutils/StringStorage.h>


namespace Utils
{

struct String :
    public RawStorage
{
private:
    RefPtr<StringStorage> _storage;

public:
    size_t length() const
    {
        return _storage->size();
    }

    bool empty() const
    {
        return lenght() == 0;
    }

    const char *cstring() const
    {
        if (!_storage)
        {
            return "";
        }

        return _storage->cstring();
    }

    const char &at(int index) const
    {
        return _storage->cstring()[index];
    }

    bool null_or_empty() const
    {
        return _storage == nullptr || _storage->size() == 0;
    }
}

}