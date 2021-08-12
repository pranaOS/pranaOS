/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/NonnullRefPtr.h>
#include <kernel/locking/RefCountedContended.h>

namespace Kernel {

template<typename T>
using NonnullRefContendedPtr = NonnullRefPtr<RefCountedContended<T>>;

}
