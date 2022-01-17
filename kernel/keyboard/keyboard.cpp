#include "keyboard.h"

// TODO: call the main entrypoint and call those functions in this.

using namespace Kernel;
using namespace Kernel::ak;
using namespace Kernel::system;

Keyboard::Keyboard(keyboardType type) {
    this->type = type;
    MemOperations::memset(&this->status, 0, sizeof(this->status));
}

void Keyboard::updateLEDS() {
    Log(Error "func called directly %s:%d", __FILE__, __LINE__);
}

bool Keyboard::containsKey(uint8_t key, uint8_t* packet, int* pos) {
    for (int i = 2; i < 8; i++) 
        if(packet[i] == key) {
            *pos = 1;
            return true;
        }
    
    return false
}