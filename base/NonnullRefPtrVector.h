/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/NonnullPtrVector.h>
#include <base/NonnullRefPtr.h>

namespace Base {

template<typename T, size_t inline_capacity>
class NonnullRefPtrVector : public NonnullPtrVector<NonnullRefPtr<T>, inline_capacity> {
using NonnullPtrVector<NonnullRefPtr<T>, inline_capacity>::NonnullPtrVector;
};

}

using Base::NonnullRefPtrVector;