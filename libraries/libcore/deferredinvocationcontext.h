/**
 * @file deferredinvocationcontext.h
 * @author Krisna Pranav
 * @brief deferred invocation context
 * @version 6.0
 * @date 2024-10-25
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <libcore/object.h>

namespace Core
{
    class DeferredInvocationContext final : public Core::Object 
    {
        /**
         * @brief Construct a new c object object
         * 
         */
        C_OBJECT(DeferredInvocationContext)
    private:
        /**
         * @brief Construct a new Deferred Invocation Context object
         * 
         */
        DeferredInvocationContext() { }
    }; // class DeferredInvocationContext final : public Core::Object 
} // namespace Core 