/**
 * @file scope_guard.h
 * @author Krisna Pranav
 * @brief scope_guard
 * @version 6.0
 * @date 2023-07-01
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/stdlibextra.h>

namespace Mods
{
    /**
     * @tparam Callback 
     */
    template <typename Callback>
    class ScopeGuard
    {
    public:
        /**
         * @brief Construct a new ScopeGuard object
         * 
         * @param callback 
         */
        ScopeGuard(Callback callback)
            : m_callback(move(callback))
        {
        }

        /**
         * @brief Destroy the ScopeGuard object
         * 
         */
        ~ScopeGuard()
        {
            m_callback();
        }

    private:
        Callback m_callback;
    }; // class ScopeGuard

    /**
     * @tparam Callback 
     */
    template <typename Callback>
    class ArmedScopeGuard
    {
    public:
        /**
         * @brief Construct a new ArmedScopeGuard object
         * 
         * @param callback 
         */
        ArmedScopeGuard(Callback callback)
            : m_callback(move(callback))
        {
        }

        /**
         * @brief Destroy the ArmedScopeGuard object
         * 
         */
        ~ArmedScopeGuard()
        {
            if(m_armed)
                m_callback();
        }

        void disarm()
        {
            m_armed = false;
        }

    private:
        Callback m_callback;
        bool m_armed{true};
    }; // class ArmedScopeGuard
} // namespace Mods

using Mods::ArmedScopeGuard;
using Mods::ScopeGuard;