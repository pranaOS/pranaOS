/**
 * @file gpio.h
 * @author Krisna Pranav
 * @brief gpio
 * @version 6.0
 * @date 2025-03-29
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

/**
 * @param mask 
 */
void gpio_irq_enable(u64 mask);

/**
 * @param mask 
 */
void gpio_irq_clear(u64 mask);