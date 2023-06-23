/**
 * @file PIT.cpp
 * @author Krisna Pranav
 * @brief PIT
 * @version 1.0
 * @date 2023-06-23
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include "../kstd/kstddef.h"
#include "PIT.h"
#include "../io.h"
#include "timemanager.h"

/**
 * @brief Construct a new PIT::PIT object
 * 
 * @param manager 
 */
PIT::PIT(TimeManager* manager): TimeKeeper(manager), IRQHandler(PIT_IRQ) {
	auto divisor = (uint16_t)(1193180u / PIT_FREQUENCY);

	uint8_t ocw = 0;
	ocw = (ocw & ~0xEu) | 0x6u;
	ocw = (ocw & ~0x30u) | 0x30u;
	ocw = (ocw & ~0xC0u) | 0u;
	IO::outb(PIT_CMD, ocw);

	write(divisor & 0xffu, 0);
	write((divisor >> 8u) & 0xffu, 0);
}