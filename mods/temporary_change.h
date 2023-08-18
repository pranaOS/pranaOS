/**
 * @file temporary_change.h
 * @author Krisna Pranav
 * @brief TemporaryChange
 * @version 6.0
 * @date 2023-07-04
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

namespace Mods {

    template<typename T>
    class TemporaryChange {
    public:

        /**
         * @brief Construct a new Temporary Change object
         * 
         * @param variable 
         * @param value 
         */
        TemporaryChange(T& variable, T value) : m_variable(variable) , m_old_value(variable)
        {
            m_variable = value;
        }

        /**
         * @brief Destroy the Temporary Change object
         * 
         */
        ~TemporaryChange() { 
            m_variable = m_old_value; 
        }

    private:
        T& m_variable;
        T m_old_value;
    };

}

using Mods::TemporaryChange;
