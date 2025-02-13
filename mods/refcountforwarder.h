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
    template <typename T>
    class RefCountForwarder
    {
    public:
        void ref()
        {
            m_ref_count_target.ref();
        }

        void unref()
        {
            m_ref_count_target.unref();
        }

        /**
         * @return T& 
         */
        T& ref_count_target()
        {
            return m_ref_count_target;
        }

        /**
         * @return T const& 
         */
        T const& ref_count_target() const
        {
            return m_ref_count_target;
        }

    protected:
        /**
         * @brief Construct a new RefCountForwarder object
         * 
         * @param target 
         */
        RefCountForwarder(T& target)
            : m_ref_count_target(target)
        {
        }

    private:
        T& m_ref_count_target;
    }; // class RefCountForwarder
} // namespace Mods

using Mods::RefCountForwarder;