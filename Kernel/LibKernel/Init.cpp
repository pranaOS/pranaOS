//
//  Init.cpp
//  Kernel
//
//  Created by Pranav on 28/06/22.
//

#include "Init.hpp"
#include <pranaLib/Common.hpp>

namespace LibKernel {
    typedef void (*constructor)();
    extern "C" constructor start_ctors;
    extern "C" constructor end_ctors;
}


