/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/RefPtr.h>
#include <kernel/locking/RefCountedContended.h>

namespace Kernel {

template<typename T>
using RefContendedPtr = RefPtr<RefCountedContended<T>>;

}
