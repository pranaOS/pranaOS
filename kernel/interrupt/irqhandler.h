/**
* @file irqhandler.h
* @author Krisna Pranav
* @brief irqhandler
* @version 1.0
*
* @copyright Copyright (c) 2023 pranaOS Developers, Krisna Pranav
*
 */

#pragma once

#include "../kstd/kstddef.h"

class IRQHandler {
  public:
    void handle(Registers *regs);
    bool sent_eoi();
    virtual bool mark_in_irq();

  protected:
    virtual void handle_irq(Registers* regs) = 0;

  private:
    int _irq = 0;
    bool _sent_eoi = false;
};