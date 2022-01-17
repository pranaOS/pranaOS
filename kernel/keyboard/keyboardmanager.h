#pragma once

#include "keyboard.h"
#include <ak/list.h>
#include <memory/fifostream.h>

namespace Kernel {
    class keyboardManager : public FIFOStream {
    public:
        List<Keyboard*> keyboards;
        keyboardStatus sharedStatus;

    private:
        uint8_t convertToPS2(uint32_t key);

    public:
        keyboardManager();
        void handleKeyChange(Keyboard* src, uint32_t key, bool pressed);
    };
}