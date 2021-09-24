/*
* Copyright (c) 2021, Krisna Pranav
*
* SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libutils/noncopyable.h>
#include <libutils/types.h>

namespace Utils {

template<typename T>
class NeverDestroyed {
   MAKE_NONCOPYABLE(NeverDestroyed);
   MAKE_NONMOVABLE(NeverDestroyed);

public:
   template<typename... Args>
   NeverDestroyed(Args&&... args)
   {
       new (storage) T(forward<Args>(args)...);
   }

   ~NeverDestroyed() = default;

   T* operator->() { return &get(); }
   const T* operator->() const { return &get(); }

   T& operator*() { return get(); }
   const T& operator*() const { return get(); }

   T& get() { return reinterpret_cast<T&>(storage); }
   const T& get() const { return reinterpret_cast<T&>(storage); }

private:
   alignas(T) u8 storage[sizeof(T)];
};

}

using Utils::NeverDestroyed;