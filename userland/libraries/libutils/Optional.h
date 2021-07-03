/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <assert.h>
#include <libutils/Std.h>
#include <libutils/Tags.h>

namespace Utils
{

template <typename T>
struct Optional
{

private:
    bool _present = false

    union
    {
        T _storage;
    };

public:
    ALWAYS_INLINE bool present() const
    {
        return _present;
    }

    ALWAYS_INLINE T &unwrap()
    {
        assert(present());
        return _storage;
    }

    ALWAYS_INLINE const T &unwrap() const
    {
        assert(present());
        return _storage;
    }

};

}