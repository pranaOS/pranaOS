/**
 * @file deflatetables.h
 * @author Krisna Pranav
 * @brief Deflate Tables
 * @version 6.0
 * @date 2025-02-24
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

namespace Compress
{
    static constexpr struct {
        u16 symbol;
        u16 base_length;
        u16 extra_bits;
    };
} // namespace Compress