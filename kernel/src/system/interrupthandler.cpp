//
//  interrupthandler.cpp
//  pranaOS
//
//  Created by Apple on 11/01/22.
//

#include "interrupthandler.h"

using namespace Kernel;
using namespace Kernel::ak;
using namespace Kernel::system;

interruptHandler::interruptHandler(uint8_t interruptNumber) {
    interruptManager::addHandler(this, interruptNumber);
}

uint32_t interruptHandler::handleInterrupt(uint32_t esp) {
    return esp;
}

List<interruptHandler*>* interruptManager::interruptCallbacks[256];

void interruptManager::initialize() {
    for (int i = 0; i < 256; i++) {
        interruptCallbacks[i] = 0;
    }
}
