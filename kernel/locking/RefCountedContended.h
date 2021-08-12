/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/RefCounted.h>
#include <kernel/locking/ContendedResource.h>
#include <kernel/locking/Mutex.h>

namespace Kernel {

template<typename T>
class RefCountedContended : public ContendedResource
    , public Base::RefCountedBase {
    BASE_MAKE_NONCOPYABLE(RefCountedContended);
    BASE_MAKE_NONMOVABLE(RefCountedContended);

protected:
    using LockedShared = LockedResource<T const, LockMode::Shared>;
    using LockedExclusive = LockedResource<T, LockMode::Exclusive>;

    LockedShared lock_shared() const { return LockedShared(static_cast<T const*>(this), this->ContendedResource::m_mutex); }
    LockedExclusive lock_exclusive() { return LockedExclusive(static_cast<T*>(this), this->ContendedResource::m_mutex); }

public:
    RefCountedContended() = default;

    bool unref() const
    {
        auto new_ref_count = deref_base();
        if (new_ref_count == 0) {
            Base::call_will_be_destroyed_if_present(static_cast<T*>(const_cast<RefCountedContended*>(this)->lock_exclusive().get()));
            delete static_cast<const T*>(this);
            return true;
        } else if (new_ref_count == 1) {
            Base::call_one_ref_left_if_present(static_cast<T*>(const_cast<RefCountedContended*>(this)->lock_exclusive().get()));
        }
        return false;
    }

    template<typename Callback>
    decltype(auto) with_shared(Callback callback) const
    {
        auto lock = lock_shared();
        return callback(*lock);
    }

    template<typename Callback>
    decltype(auto) with_exclusive(Callback callback)
    {
        auto lock = lock_exclusive();
        return callback(*lock);
    }

    template<typename Callback>
    void for_each_shared(Callback callback) const
    {
        with_shared([&](const auto& value) {
            for (auto& item : value)
                callback(item);
        });
    }

    template<typename Callback>
    void for_each_exclusive(Callback callback)
    {
        with_exclusive([&](auto& value) {
            for (auto& item : value)
                callback(item);
        });
    }
};

}
