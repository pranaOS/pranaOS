#pragma once

#include <ak/types.h>

namespace Kernel {

    struct internalKeyboardStatus {
        bool leftShift;
        bool rightShift;
        bool alt;
        bool altGr;
        bool leftControl;
        bool rightControl;
    };

    struct keyboardStatus {
        bool CapsLock;
        bool NumLock;
    };
        
    enum keyboardType {
        PS2,
        USB
    };

    class Keyboard {
    public:
        keyboardType type;
        internalKeyboardStatus status;

    public:
        Keyboard(keyboardType type);
        virtual void updateLEDS();
        bool containsKey(ak::uint8_t key, ak::uint8_t* packet, int* pos);
    };
}