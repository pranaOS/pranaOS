/**
 * @file allocationstrategy.h
 * @author Krisna Pranav
 * @brief Allocation Strategy
 * @version 6.0
 * @date 2024-10-17
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

namespace Kernel
{
enum class AllocationStrategy
{
    Reserve = 0,
    AllocateNow,
    None
}; // enum class AllocationStrategy
} // namespace Kernel
