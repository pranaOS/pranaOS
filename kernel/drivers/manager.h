//
// Created by Krisna Pranav on 19/01/22.
//

#pragma once

#include <ak/types.h>
#include <ak/list.h>
#include <system/console.h>
#include "driver.h"

namespace Kernel::Driver {
    class driverManager {
      public:
        driverManager();
        void addDriver(Kernel::Driver::driver* drv);

      private:
        List<Driver*> driverList;
    };
}