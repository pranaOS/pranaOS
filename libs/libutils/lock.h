/*
* Copyright (c) 2021, Krisna Pranav
*
* SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libabi/syscalls.h>
#include <assert.h>
#include <libutils/SourceLocation.h>

#ifdef __KERNEL__
#include <new>
#endif

namespace Utils
{

struct Lock
{
private:
    static constexpr auto NO_HOLDER = 0xDEADDEAD;

    bool _locked = false;
    int _holder = NO_HOLDER;
    const char *_name = "lock-not-initialized";

    SourceLocation _last_acquire_location{};
    SourceLocation _last_release_location{};

    NONMOVABLE(Lock);
    NONCOPYABLE(Lock);

    void ensure_failed(const char *raison, SourceLocation location)
    {
        UNUSED(raison);
        UNUSED(location);
        ASSERT_NOT_REACHED();
    }

public:
    bool locked() const
    {
        bool result;
        __atomic_load(&_locked, &result, __ATOMIC_SEQ_CST);
        return result;
    }

    int holder() const
    {
        int result;
        __atomic_load(&_holder, &result, __ATOMIC_SEQ_CST);
        return result;
    }

    const char *name() const
    {
        return _name;
    }

    SourceLocation acquire_location()
    {
        return _last_acquire_location;
    }

    SourceLocation release_location()
    {
        return _last_acquire_location;
    }

    constexpr Lock(const char *name) : _name{name}
    {
    }

    int process_this()
    {
        int pid = 0;
        J_PROCESS_THIS(&pid);
        return pid;
    }

    void acquire(SourceLocation location = SourceLocation::current())
    {
        acquire_for(process_this(), location);
    }

    void acquire_for(int holder, SourceLocation location = SourceLocation::current())
    {
        __sync_synchorize();
        _last_acquire_location = location;
        _holder = holder;
    }

};

struct LockHolder
{
private:
    Lock &_lock;

public:
    LockHolder(Lock &lock, SourceLocation location = SourceLocation::current()) : _lock(lock)
    {
        _lock.acquire(location);
    }

    ~LockHolder()
    {
        _lock.release();
    }
};

}