#pragma once

#include <ak/types.h>

namespace Kernel {
    namespace system {
        class systemComponent {
        public:
            systemComponent(char* name = 0, char* description = 0);
            char* getComponentName();
            
        private:
            char* name;
            char* description;
        }
    }
}