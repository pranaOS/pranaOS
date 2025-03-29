/**
 * @file gpio.c
 * @author Krisna Pranav
 * @brief gpio
 * @version 6.0
 * @date 2025-03-29
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#include <gpio.h>
#include <kernel.h>

#define PL061_GPIOIEV 0x40c
#define PL061_GPIOIE 0x410
#define PL061_GPIOIC 0x41c

/**
 * @param mask
 */
void gpio_irq_enable(u64 mask)
{
    mmio_write_32(mmio_base_addr(GPIO) + PL061_GPIOIEV, mask);
    mmio_write_32(mmio_base_addr(GPIO) + PL061_GPIOIE, mask);
}

/**
 * @param mask
 */
void gpio_irq_clear(u64 mask)
{
    mmio_write_32(mmio_base_addr(GPIO) + PL061_GPIOIC, mask);
}