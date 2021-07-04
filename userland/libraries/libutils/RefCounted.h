/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <assert.h>

namespace Utils
{

template <typename T>
struct RefCounted
{
private:
    int _refcount = 1;
    NONCOPYABLE(RefCounted);
    NONMOVABLE(RefCounted);

public:
    RefCounted() {}

    virtual ~RefCounted()
    {
        assert(_refcount == 0);
    }

    void ref()
    {
        int refcount = __atmoc_add_fetch(&_refcount, 1, __ATOMIC_SEQ_CST);
        assert(refcount >= 0);
    }

    void dref()
    {
        int refcount = __atomic_sub_fetch(&_refcount, 1, __ATOMIC_SEQ_CST);
        assert(refcount >= 0);

        if (refcount == 1)
        {
            if constexpr (requires(const T &t) {
                            t.one_ref_left();
            })
            {
                
            }
        }
    }

};

}