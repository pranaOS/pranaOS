/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

/* includes */
#include <libutils/assertions.h>
#include <libutils/stdextras.h>
#include <libutils/types.h>
#include <libutils/kmalloc.h>

namespace Utils {

template <typename T>
class [[nodiscard]] Optional {
public:
    using ValueType = T;

    ALWAYS_INLINE Optional() = default;
};

}