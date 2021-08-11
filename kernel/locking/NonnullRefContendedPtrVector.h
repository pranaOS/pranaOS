/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <Base/NonnullPtrVector.h>
#include <kernel/locking/NonnullRefContendedPtr.h>

namespace Kernel {

template<typename T, size_t inline_capacity = 0>
using NonnullRefContendedPtrVector = AK::NonnullPtrVector<NonnullRefContendedPtr<T>, inline_capacity>;

}
