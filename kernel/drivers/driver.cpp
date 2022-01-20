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

char* driver::getDriverDescription() {
    return this->driverDescription;
}

bool driver::initialize() {
    sendLog(Error, "driver function error please report about this problem %s%d", __FILE__, __LINE__);
    return false;
}