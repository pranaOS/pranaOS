/**
 * @file listedrefcount.h
 * @author Krisna Pranav
 * @brief Listed Ref Count
 * @version 6.0
 * @date 2024-10-05
 * 
 * @copyright Copyright (c) 2021-2024, pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/refcounted.h>

namespace Kernel 
{

    enum class LockType 
    {
        Spinlock,
        Mutex,
    }; // enum class LockType

    /**
     * @tparam T 
     * @tparam Lock 
     */
    template<typename T, LockType Lock>
    class ListedRefCounted : public RefCountedBase 
    {
    public:
        bool unref() const
        {
            auto* that = const_cast<T*>(static_cast<T const*>(this));

            auto callback = [&](auto& list) {
                auto new_ref_count = deref_base();
                if (new_ref_count == 0) {
                    list.remove(const_cast<T&>(*that));
                    if constexpr (requires { that->revoke_weak_ptrs(); }) {
                        that->revoke_weak_ptrs();
                    }
                    if constexpr (requires { that->remove_from_secondary_lists(); })
                        that->remove_from_secondary_lists();
                }
                return new_ref_count;
            };

            RefCountType new_ref_count;

            if constexpr (Lock == LockType::Spinlock)
                new_ref_count = T::all_instances().with(callback);
            else if constexpr (Lock == LockType::Mutex)
                new_ref_count = T::all_instances().with_exclusive(callback);

            if (new_ref_count == 0) {
                if constexpr (requires { that->will_be_destroyed(); })
                    that->will_be_destroyed();
                delete that;
            }

            return new_ref_count == 0;
        }
    }; // class ListedRefCounted

} // namespace Kernel