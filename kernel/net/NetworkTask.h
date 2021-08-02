/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

namespace Kernel {
class NetworkTask {
public:
    static void spawn();
    static bool is_current();
};
}