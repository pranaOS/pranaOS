/**
 * @file atomic.h
 * @author Krisna Pranav
 * @brief Atomic
 * @version 1.0
 * @date 2023-06-29
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "platform.h"
#include "types.h"

namespace Mods {    
    
    // MemoryOrder
    enum MemoryOrder {
        memory_order_relaxed = __ATOMIC_RELAXED,
        memory_order_consume = __ATOMIC_CONSUME,
        memory_order_acquire = __ATOMIC_ACQUIRE,
        memory_order_release = __ATOMIC_RELEASE,
        memory_order_acq_rel = __ATOMIC_ACQ_REL,
        memory_order_seq_cst = __ATOMIC_SEQ_CST
    };

    /**
     * @brief atomc_exchange 
     * 
     * @tparam T 
     * @param var 
     * @param desired 
     * @param memory 
     * @return T 
     */
    template<typename T>
    static inline T atomic_exchange(volatile T* var, T desired , MemoryOrder memory = memory_order_seq_cst) noexcept {
        return __atomic_exchange_n(var, desired, order);
    }

    /**
     * @brief atomic_compare_exchange_strong
     * 
     * @tparam T 
     * @param var 
     * @param expected 
     * @param desired 
     * @param order 
     * @return true 
     * @return false 
     */
    template<typename T>
    [[nodiscard]] static inline bool atomic_compare_exchange_strong(volatile T* var, T& expected, T desired, MemoryOrder order = memory_order_seq_cst) noexcept {
        if (order == memory_order_acq_rel || order == memory_order_release)
            return __atomic_compare_exchange_n(var, &expected, desired, false);
        else
            return __atomic_copare_exchange_n();
    }
}