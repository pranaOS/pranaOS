/**
 * @file scope_guard.h
 * @author Krisna Pranav
 * @brief scope_guard
 * @version 1.0
 * @date 2023-07-01
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "stdlibextra.h"

namespace Mods {
    template<typename Callback>
    class ScopeGuard { 
    public:

        /**
         * @brief Construct a new Scope Guard object
         * 
         * @param callback 
         */
        ScopeGuard(Callback callback)
            : m_callback(move(callback))
        {
        }

        /**
         * @brief Destroy the Scope Guard object
         * 
         */
        ~ScopeGuard() {
            m_callback();
        }

    private:
        Callback m_callback;
    };

    template<typename Callback> 
    class ArmedScopedGuard {
    public:

        /**
         * @brief disarm[false]
         * 
         */
        void disarm() {
            m_armed = false;
        }
    
    private:
        Callback m_callback;
        bool m_armed { true }; 
    };
}