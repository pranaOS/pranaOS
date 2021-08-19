/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */
#pragma once

#include <EP/NonnullPtrVector.h>
#include <EP/NonnullRefPtr.h>

namespace EP {

template<typename T, size_t inline_capacity>
class NonnullRefPtrVector : public NonnullPtrVector<NonnullRefPtr<T>, inline_capacity> {
    using NonnullPtrVector<NonnullRefPtr<T>, inline_capacity>::NonnullPtrVector;
};

}

using EP::NonnullRefPtrVector;
