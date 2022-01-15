#pragma once

#include <system/input/keyboard.h>
#include <ak/list.h>

namespace Kernel {
    namespace system {
        class keyboardManager {
        public:
            List<Keyboard*> keyboards;

            keyboardStatus sharedStatus;

        
        private:
            uint8_t convertToPS2(uint32_t key);

        public:
            keyboardManager();

            void handleKeyChange(Keyboard* src, uint32_t key, int pressed);
        }
    }
}