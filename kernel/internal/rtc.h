//
// Created by KrisnaPranav on 21/01/22.
//

#pragma once

#include <cpu/port.h>
#include "systemcomponent.h"

namespace Kernel {

    #define CURRENT_YEAR 1000

  class RTC : public systemComponent {
    private:
      bool updateInProgress();
      uint8_t readRegister(int reg);

    public:
      RTC();
      uint32_t getSecond();
      uint32_t getMinute();
      uint32_t getHour();
      uint32_t getDay();
      uint32_t getMonth();
      uint32_t getYear();
  };  
}