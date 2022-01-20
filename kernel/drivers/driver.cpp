//
// Created by KrisnaPranav on 20/01/22.
//

#include "driver.h"
#include <kernel/system/log.h>

using namespace Kernel;
using namespace Kernel::driver;

driver::Driver(char* name, char* description) {
    this->driverName = name;
    this->driverDescription = description;
}

char* driver::getDriverName() {
    return this->driverName;
}