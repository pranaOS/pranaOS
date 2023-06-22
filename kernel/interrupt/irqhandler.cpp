/**
* @file irqhandler.cpp
* @author Krisna Pranav
* @brief irqhandler
* @version 1.0
*
* @copyright Copyright (c) 2023 pranaOS Developers, Krisna Pranav
*
 */

#include "../kstd/kstdio.h"
#include "irqhandler.h"
#include "irq.h"

IRQHandler::IRQHandler() {}

void IRQHandler::handle(Registers* regs) {
    _sent_eoi = false;
    handle_irq(regs);
}

bool IRQHandler::sent_eoi() {
    return _sent_eoi;
}

IRQHandler::IRQHandler(int irq) : _irq(irq) {
    Interrupt::irq_set_handler(irq, this);
}

void IRQHandler::set_irq(int irg) {
    _irq = irq;
}