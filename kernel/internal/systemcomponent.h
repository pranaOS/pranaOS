#pragma once

#include <ak/types.h>

namespace Kernel {
    class systemComponent {
    public:
        systemComponent(char* name = 0, char* description = 0);

        char* getComponentName();
        char* getComponentDescription();

    private:
        char* Name;
        char* Description;
    };
}