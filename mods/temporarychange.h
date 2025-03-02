/**
 * @file temporarychange.h
 * @author Krisna Pranav
 * @brief TemporaryChange
 * @version 6.0
 * @date 2023-06-29
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
    class TemporaryChange
    {
    public:
        /**
         * @brief Construct a new Temporary Change object
         * 
         * @param variable 
         * @param value 
         */
        TemporaryChange(T& variable, T value)
            : m_variable(variable), m_old_value(variable)
        {
            m_variable = value;
        }

        /**
         * @brief Destroy the Temporary Change object
         * 
         */
        ~TemporaryChange()
        {
            m_variable = m_old_value;
        }

    private:
        T& m_variable;
        T m_old_value;
    }; // class TemporaryChange
} // namespace Mods

using Mods::TemporaryChange;