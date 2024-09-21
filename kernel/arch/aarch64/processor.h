/**
 * @file processor.h
 * @author Krisna Pranav
 * @brief Processor
 * @version 6.0
 * @date 2024-09-21
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/array.h>
#include <mods/concept.h>
#include <mods/function.h>
#include <mods/types.h>
#include <kernel/arch/processorspecificdataid.h>

namespace Kernel 
{

    class Thread;

    struct [[gnu::aligned(16)]] FPUState
    {
        u8 buffer[512];
    };

    class Processor 
    {
    public:
        void set_specific(ProcessorSpecificDataID, void*) { }

        /**
         * @brief Get the specific object
         * 
         * @tparam T 
         * @return T* 
         */
        template<typename T>
        T* get_specific() 
        { 
            return 0; 
        }

        ALWAYS_INLINE static void pause() { }
        ALWAYS_INLINE static void wait_check() { }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE static bool is_initialized()
        {
            return false;
        }

        ALWAYS_INLINE static u32 current_id()
        {
            return 0;
        }

        ALWAYS_INLINE static Thread* current_thread()
        {
            return 0;
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE static FlatPtr current_in_irq()
        {
            return 0;
        }

        ALWAYS_INLINE static void enter_critical() { }
        ALWAYS_INLINE static void leave_critical() { }

        ALWAYS_INLINE static u32 in_critical()
        {
            return 0;
        }

        /**
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE static Processor& current() 
        { 
            VERIFY_NOT_REACHED(); 
        }

        static void deferred_call_queue(Function<void()>) { }

        [[noreturn]] static void halt()
        {
            for (;;) { }
        }
    }; // class Processor

} // namespace Kernel