/*
 * Copyright (c) 2022, Krisna Pranav
 *
 * SPDX-License-Identifier: GPL-3.0 License
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
