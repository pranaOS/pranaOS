#pragma once

#include "systemcomponent.h"
#include <ak/types.h>

namespace Kernel {
    namespace system {
        #define APM_SIGNATURE 0x504D

        class APMController : public systemComponent {
        public:
            bool enabled = true;

        public:
            APMController();

            void checkAndHandleEvents();
            void setPowerState(ak::uint16_t device, ak::uint8_t state);
            void disableResumeTimer();
        };
    }
}