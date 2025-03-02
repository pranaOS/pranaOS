/**
 * @file scoped_value_rollback.h
 * @author Krisna Pranav
 * @brief ScopedValueRollBack
 * @version 6.0
 * @date 2023-07-01
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
    class ScopedValueRollback
    {
    public:
        /**
         * @brief Construct a new ScopedValueRollback object
         * 
         * @param variable 
         */
        ScopedValueRollback(T& variable)
            : m_variable(variable), m_saved_value(variable)
        {
        }

        /**
         * @brief Destroy the ScopedValueRollback object
         * 
         */
        ~ScopedValueRollback()
        {
            m_variable = m_saved_value;
        }

        /**
         * @brief Set the override rollback value object
         * 
         * @param value 
         */
        void set_override_rollback_value(const T& value)
        {
            m_saved_value = value;
        }

    private:
        T& m_variable;
        T m_saved_value;
    }; // class ScopedValueRollback
} // namespace Mods

using Mods::ScopedValueRollback;