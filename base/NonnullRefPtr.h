/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Assertions.h>
#include <base/Atomic.h>
#include <base/Format.h>
#include <base/Traits.h>
#include <base/Types.h>
#ifdef KERNEL
#    include <kernel/arch/x86/Processor.h>
#    include <kernel/arch/x86/ScopedCritical.h>
#endif

namespace Base {

template<typename T>
class OwnPtr;
template<typename T, typename PtrTraits>
class RefPtr;

template<typename T>
ALWAYS_INLINE void ref_if_not_null(T* ptr)
{
    if (ptr)
        ptr->ref();
}

template<typename T>
ALWAYS_INLINE void unref_if_not_null(T* ptr)
{
    if (ptr)
        ptr->unref();
}

template<typename T>
class NonnullRefPtr {
    template<typename U, typename P>
    friend class RefPtr;
    template<typename U>
    friend class NonnullRefPtr;
    template<typename U>
    friend class WeakPtr;

public:
    using ElementType = T;

    enum AdoptTag { Adopt };
};

}