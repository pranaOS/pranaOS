/**
 * @file deferredinvocationcontext.h
 * @author Krisna Pranav
 * @brief deferred invocation context
 * @version 6.0
 * @date 2024-10-25
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <libcore/object.h>

namespace Core 
{

    class DeferredInvocationContext final : public Core::Object 
    {
        C_OBJECT(DeferredInvocationContext)
    private:
        /**
         * @brief Construct a new DeferredInvocationContext object
         * 
         */
        DeferredInvocationContext() = default;
    }; // class DeferredInvocationContext final : public Core::Object 

} // namespace Core