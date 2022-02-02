/*
 * Copyright (c) 2022, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <ak/types.h>

namespace Kernel {
    class component {
    public:
        component(char* name = 0, char* description = 0);

        char* getComponentName();
        char* getComponentDescription();
            
    private:
        char* name;
        char* description;
    };
}
