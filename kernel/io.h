/**
 * @file io.h
 * @author Krisna Pranav
 * @brief IO
 * @version 1.0
 * @date 2023-06-19
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "kstd/unix_types.h"

namespace IO {
    /**
     * @brief wait
     * 
     */
	void wait();

    /**
     * @brief outb
     * 
     * @param port 
     * @param value 
     */
	void outb(uint16_t port, uint8_t value);

    /**
     * @brief outw
     * 
     * @param port 
     * @param value 
     */
	void outw(uint16_t port, uint16_t value);

    /**
     * @brief outl
     * 
     * @param port 
     * @param value 
     */
	void outl(uint16_t port, uint32_t value);

    /**
     * @brief inb
     * 
     * @param port 
     * @return uint8_t 
     */
	uint8_t inb(uint16_t port);

    /**
     * @brief inw
     * 
     * @param port 
     * @return uint16_t 
     */
	uint16_t inw(uint16_t port);

    /**
     * @brief inl
     * 
     * @param port 
     * @return uint32_t 
     */
	uint32_t inl(uint16_t port);

    /**
     * @brief wait
     * 
     * @param us 
     */
	inline void wait(size_t us) {
		while(us--)
			inb(0x80);
	}
};