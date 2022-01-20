//
// Created by KrisnaPranav on 20/01/22.
//

#include "manager.h"
#include <kernel/system/log.h>

using namespace Kernel;

driverManager::driverManager() {
    this->driverList.clear();
}

void driverManager::addDriver(driver* drv) {
    this->driverList.push_back(drv);
}

void driverManager::activate() {

}