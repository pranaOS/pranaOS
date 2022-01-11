//
//  component.h
//  pranaOS
//
//  Created by Apple on 11/01/22.
//

#pragma once

#include <ak/types.h>

namespace Kernel {
    namespace system {
        class componenet {
        public:
            componenet(char* name = 0, char* description = 0);
            
            char* getComponenetName();
            char* getComponenetDescription();
            
        private:
            char* name;
            char* description;
        }
    }
}
