//
// Created by KrisnaPranav on 21/01/22.
//

#pragma once

#include <cpu/port.h>
#include "systemcomponent.h"

namespace Kernel {

  #define CURRENT_YEAR 2020

  class RTC : public systemComponent {
    private:
      bool updateInProgress();
      ak::uint8_t readRegister(int reg);

    public:
      RTC();
      ak::uint32_t getSecond();
      ak::uint32_t getMinute();
      ak::uint32_t getHour();
      ak::uint32_t getDay();
      ak::uint32_t getMonth();
      ak::uint32_t getYear();
  };  
}