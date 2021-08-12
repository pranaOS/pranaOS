/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/String.h>
#include <base/Vector.h>
#include <kernel/KBuffer.h>

namespace Kernel {

typedef void* (*ModuleInitPtr)();
typedef void* (*ModuleFiniPtr)();

struct Module {
    String name;
    Vector<KBuffer> sections;

    ModuleInitPtr module_init { nullptr };
    ModuleFiniPtr module_fini { nullptr };
};

}
