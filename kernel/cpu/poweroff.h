#pragma once

#include <port.h>

namespace Kernel {
    class powerOff {
    public:
        /**
         * @brief initialize poweroff function
         * 
         */
        static void initialize();

        /**
         * @brief send poweroff 
         * 
         */
        static void poweroff();
    };
}
