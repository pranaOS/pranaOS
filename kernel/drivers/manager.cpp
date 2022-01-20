//
// Created by KrisnaPranav on 20/01/22.
//

#include "manager.h"
#include "driver.h"
#include <kernel/system/log.h>

using namespace Kernel;

driverManager::driverManager() {
    this->driverList.clear();
}

void driverManager::addDriver(driver* drv) {
    this->driverList.push_back(drv);
}

void driverManager::activate() {
    for(int i = 0; i < driverList.size(); i++) {
        Log(Info, "Initializing Driver... %s", driverList[i]->getDriverName());

        if(driverList[i]->initialize() == false)
            Log(Error, "Initializing driver failed: %s", driverList[i]->getDriverName());
    }
}