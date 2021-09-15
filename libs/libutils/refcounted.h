/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

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
    RefCounted()
    {
    }

    virtual ~RefCounted()
    {
        assert(_refcount == 0);
    }

    void ref()
    {
        int refcount = __atomic_add_fetch(&_refcount, 1, __ATOMIC_SEQ_CST);
        assert(refcount >= 0);
    }

};

struct AnyRef : public RefCounted<AnyRef>
{
    virtual ~AnyRef() = default;
};

}