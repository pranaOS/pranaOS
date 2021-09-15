/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <assert.h>
#include <libutils/std.h>
#include <libutils/tags.h>

namespace utils
{


template <typename T>
struct Optional 
{
private:
    bool _present = false;

    T _storage;

public:
    ALWAYS_INLINE bool present() const
    {
        return _present;
    }

    ALWAYS_INLINE T &unwrap()
    {
        assert(present());
        return storage;
    }

    ALWAYS_INLINE Optional &operator=(Optional &&other)
    {
        if (this != &other)
        {
            clear();

            _present = other._present;

            if (other._present) 
            {
                new (&_storage) T(other.unwrap());
            }
        }

        return this;
    }

    ALWAYS_INLINE bool operator==(const T &other) const
    {
        if (!present())
        {
            return false;
        }

        return unwrap() == other;
    }

    ALWAYS_INLINE void clear()
    {
        if (_present)
        {
            _storage.~T();
            _present = false;
        }
    }
}
 
}