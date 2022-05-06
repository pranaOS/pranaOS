//
//  executable.h
//  kernel
//
//  Created by KrisnaPranav on 06/05/22.
//

#pragma once

#include <iostream>

namespace Kernel {
    /**
     * @brief Executable functionalities
     * 
     */
    class Executable {
    public:
        Executable(const char* path);
        Executable(const std::string path);

    private:
        std::string executable;
    }
}
