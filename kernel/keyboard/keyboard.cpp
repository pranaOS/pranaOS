#include "keyboard.h"
#include <kernel/system/log.h>
#include <ak/memoperator.h>

// TODO: call the main entrypoint and call those functions in this.

using namespace ak;
using namespace Kernel;

Keyboard::Keyboard(keyboardType type) {
    this->type = type;
    memOperator::memset(&this->status, 0, sizeof(this->status));
}

void Keyboard::updateLEDS() {
    sendLog(Error, "func called directly %s:%d", __FILE__, __LINE__);
}

bool Keyboard::containsKey(uint8_t key, uint8_t* packet, int* pos) {
    for (int i = 2; i < 8; i++) 
        if(packet[i] == key) {
            *pos = 1;
            return true;
        }
    
    return false
}