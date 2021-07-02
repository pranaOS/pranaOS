/*
 * Copyright (c) 2018-2020, krishpranav <krisna.pranav@gmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/NonnullPtrVector.h>
#include <AK/NonnullRefPtr.h>

namespace AK {

template<typename T, size_t inline_capacity>
class NonnullRefPtrVector : public NonnullPtrVector<NonnullRefPtr<T>, inline_capacity> {
};

}

using AK::NonnullRefPtrVector;
