/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */
#pragma once

#include <EP/NonnullOwnPtr.h>
#include <EP/NonnullPtrVector.h>

namespace EP {

template<typename T, size_t inline_capacity>
class NonnullOwnPtrVector : public NonnullPtrVector<NonnullOwnPtr<T>, inline_capacity> {
};

}

using EP::NonnullOwnPtrVector;
