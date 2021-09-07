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

    int refcount()
    {
        return _refcount;
    }
};

struct AnyRef : public RefCounted<AnyRef>
{
    virtual ~AnyRef() = default;
};

template <typename T>
T *ref_if_not_null(T *ptr)
{
    if (ptr)
    {
        ptr->ref();
    }

    return ptr;
}

// FIXME: not null deref :) needec

}