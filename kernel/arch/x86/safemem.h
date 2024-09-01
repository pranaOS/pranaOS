/**
 * @file safemem.h
 * @author Krisna Pranav
 * @brief Safe Memory
 * @version 6.0
 * @date 2024-09-01
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/atomic.h>
#include <mods/optional.h>
#include <mods/types.h>

#pragma once

namespace Kernel 
{

    struct RegisterState;

    /**
     * @param dest_ptr
     * @param src_ptr
     * @param n
     * @param fault_at
     * 
     */
    [[nodiscard]] bool safe_memcpy(void* dest_ptr, const void* src_ptr, size_t n, void*& fault_at) __attribute__((used));

    /**
     * @param str
     * @param max_n
     * @param fault_at
     * 
     */
    [[nodiscard]] ssize_t safe_strnlen(const char* str, size_t max_n, void*& fault_at) __attribute__((used));

    /**
     * @param dest_ptr
     * @param c
     * @param n
     * @param fault_at
     */
    [[nodiscard]] bool safe_memset(void* dest_ptr, int c, size_t n, void*& fault_at) __attribute__((used));

    /**
     * @param var
     * @param val
     */
    [[nodiscard]] Optional<u32> safe_atomic_fetch_add_relaxed(volatile u32* var, u32 val) __attribute__((used));

    /**
     * @param var
     * @param val
     * 
     */
    [[nodiscard]] Optional<u32> safe_atomic_exchange_relaxed(volatile u32* var, u32 val) __attribute__((used));

    /**
     * @param var
     */
    [[nodiscard]] Optional<u32> safe_atomic_load_relaxed(volatile u32* var) __attribute__((used));

    /**
     * @param var
     * @param  val
     * 
     */
    [[nodiscard]] bool safe_atomic_store_relaxed(volatile u32* var, u32 val) __attribute__((used));

    /**
     * @param var
     * @param expected
     * @param val
     */
    [[nodiscard]] Optional<bool> safe_atomic_compare_exchange_relaxed(volatile u32* var, u32& expected, u32 val) __attribute__((used));

    /**
     * @param var 
     * @param val 
     * @return ALWAYS_INLINE 
     */
    [[nodiscard]] ALWAYS_INLINE Optional<u32> safe_atomic_fetch_and_relaxed(volatile u32* var, u32 val)
    {
        auto expected_value = safe_atomic_load_relaxed(var);

        if (!expected_value.has_value())
            return {}; 

        u32& expected = expected_value.value();

        for (;;) {
            auto result = safe_atomic_compare_exchange_relaxed(var, expected, expected & val);
            if (!result.has_value())
                return {}; 
            if (result.value())
                return expected; 

            Mods::atomic_thread_fence(Mods::MemoryOrder::memory_order_acquire);
        }
    }

    /**
     * @param var 
     * @param val 
     * @return ALWAYS_INLINE 
     */
    [[nodiscard]] ALWAYS_INLINE Optional<u32> safe_atomic_fetch_and_not_relaxed(volatile u32* var, u32 val)
    {
        auto expected_value = safe_atomic_load_relaxed(var);

        if (!expected_value.has_value())
            return {}; 

        u32& expected = expected_value.value();
        
        for (;;) {
            auto result = safe_atomic_compare_exchange_relaxed(var, expected, expected & ~val);
            if (!result.has_value())
                return {}; 
            if (result.value())
                return expected; 

            Mods::atomic_thread_fence(Mods::MemoryOrder::memory_order_acquire);
        }
    }

    /**
     * @param var 
     * @param val 
     * @return ALWAYS_INLINE 
     */
    [[nodiscard]] ALWAYS_INLINE Optional<u32> safe_atomic_fetch_or_relaxed(volatile u32* var, u32 val)
    {
        auto expected_value = safe_atomic_load_relaxed(var);

        if (!expected_value.has_value())
            return {}; 

        u32& expected = expected_value.value();

        for (;;) {
            auto result = safe_atomic_compare_exchange_relaxed(var, expected, expected | val);
            if (!result.has_value())
                return {}; 
            if (result.value())
                return expected; 

            Mods::atomic_thread_fence(Mods::MemoryOrder::memory_order_acquire);
        }
    }

    /**
     * @param var 
     * @param val 
     * @return ALWAYS_INLINE 
     */
    [[nodiscard]] ALWAYS_INLINE Optional<u32> safe_atomic_fetch_xor_relaxed(volatile u32* var, u32 val)
    {
        auto expected_value = safe_atomic_load_relaxed(var);

        if (!expected_value.has_value())
            return {}; 

        u32& expected = expected_value.value();
        
        for (;;) {
            auto result = safe_atomic_compare_exchange_relaxed(var, expected, expected ^ val);
            if (!result.has_value())
                return {}; 
            if (result.value())
                return expected; 

            Mods::atomic_thread_fence(Mods::MemoryOrder::memory_order_acquire);
        }
    }

    /**
     * @param regs 
     * @param fault_address 
     * @return true 
     * @return false 
     */
    bool handle_safe_access_fault(RegisterState& regs, FlatPtr fault_address);

} // namespace Kernel