/**
 * @file refcountforwarder.h
 * @author Krisna Pranav
 * @brief Ref Count Forwarder
 * @version 6.0
 * @date 2025-02-13
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

namespace Mods
{
    /**
     * @tparam T 
     */
    template<typename T>
    class RefCountedForwarder
    {
    public:
        void ref() 
        {
            m_ref_count_target.ref();
        }

    protected:
        /**
         * @brief Construct a new RefCountedForwarder object
         * 
         * @param target 
         */
        RefCountedForwarder(T& target)
            : m_ref_count_target(target)
        {
        }

    private:
        T& m_ref_count_target;
    }; // class RefCountedForwarder
} // namespace Mods

using Mods::RefCountedForwarder;