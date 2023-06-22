/**
* @file PIT.h
* @author Krisna Pranav
* @brief PIT
* @version 1.0
*
* @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
*
*/

#pragma once

#define PIT_COUNTER0 0x40
#define PIT_COUNTER1 0x41
#define PIT_COUNTER2 0x42
#define PIT_CMD  0x43
#define PIT_IRQ 0
#define PIT_FREQUENCY 1000

#include "../interrupt/irqhandler.h"
#include "timekeeper.h"

class PIT: public TimeKeeper : public IRQHandler {
  public:
    PIT(TimeManager* manager);
    void handle_irq(Registers* regs) override;
    bool mark_in_req() override;

    int frequency() override;
    void enable() override;
    void disalbe() override;
    
  private:
    static void write(uint16_t data, uint8_t counter);
};