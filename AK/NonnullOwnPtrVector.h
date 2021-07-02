/*
 * Copyright (c) 2018-2020, krishpranav <krisna.pranav@gmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/NonnullOwnPtr.h>
#include <AK/NonnullPtrVector.h>

namespace AK {

template<typename T, size_t inline_capacity>
class NonnullOwnPtrVector : public NonnullPtrVector<NonnullOwnPtr<T>, inline_capacity> {
};

}

using AK::NonnullOwnPtrVector;
