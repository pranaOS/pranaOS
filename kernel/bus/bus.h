//
// Created by KrisnaPranav on 24/01/22.
//

#ifndef BUS_KERNEL_HEADER
#define BUS_KERNEL_HEADER

#include "address.h"

namespace Kernel {
    class Bus {
      public:
        void busInstall(void);
        void enable_api(Kernel::Address);
        void disable_api(Kernel::Address);

        void initialize();

      private:
        Bus();
    };
}

#endif 