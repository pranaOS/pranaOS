//
//  component.h
//  pranaOS
//
//  Created by Krisna Pranav on 11/01/22.
//

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
