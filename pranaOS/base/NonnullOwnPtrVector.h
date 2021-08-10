/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <base/NonnullOwnPtr.h>
#include <base/NonnullPtrVector.h>

namespace Base {

template<typename T, size_t inline_capacity>
class NonnullOwnPtrVector : public NonnullPtrVector<NonnullOwnPtr<T>, inline_capacity> {
};

}

using Base::NonnullOwnPtrVector;
