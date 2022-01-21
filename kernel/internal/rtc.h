//
// Created by KrisnaPranav on 21/01/22.
//

#pragma once

#include <cpu/port.h>
#include "systemcomponent.h"

namespace Kernel {
    #define CURRET_YEAR 1000

    class RTC : public systemComponent {
      public:
        RTC();

        ak::uint32_t getSecond();
        ak::uint32_t getMinute();
        ak::uint32_t getHour();
        ak::uint32_t getDay();
        
      private:
        bool updateInProgress();
        ak::uint8_t readRegister(int reg);
    };
}