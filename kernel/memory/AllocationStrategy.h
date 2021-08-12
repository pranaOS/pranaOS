/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

namespace Kernel {

enum class AllocationStrategy {
    Reserve = 0,
    AllocateNow,
    None
};

}
