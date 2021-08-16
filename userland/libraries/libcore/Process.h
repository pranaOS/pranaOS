/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/Forward.h>

namespace Core {

class Process {
public:
    static pid_t spawn(StringView path);
};

}
