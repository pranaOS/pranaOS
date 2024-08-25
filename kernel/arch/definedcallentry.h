/**
 * @file definedcallentry.h
 * @author Krisna Pranav
 * @brief defined call entry
 * @version 6.0
 * @date 2024-08-25
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/bitcast.h>
#include <mods/function.h>

namespace Kernel 
{
    struct DeferredCallEntry 
    {
        using HandlerFunction = Function<void()>;

        DeferredCallEntry* next;

        alignas(HandlerFunction) u8 handler_storage[sizeof(HandlerFunction)];
        
        bool was_allocated;

        /**
         * @return HandlerFunction& 
         */
        HandlerFunction& handler_value()
        {
            return *bit_cast<HandlerFunction*>(&handler_storage);
        }

        void invoke_handler()
        {
            handler_value()();
        }
    }; // struct DeferredCallEntry
} // namespace Kernel
