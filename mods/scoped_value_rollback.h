/**
 * @file scoped_value_rollback.h
 * @author Krisna Pranav
 * @brief ScopedValueRollBack 
 * @version 1.0
 * @date 2023-07-01
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

namespace Mods {
    template<typename T>
    class ScopedValueRollback {
    public:
        /**
         * @brief Construct a new Scoped Value Rollback object
         * 
         * @param variable 
         */
        ScopedValueRollback(T& variable) 
            : m_variable(variable)
            , m_saved_value(variable)
        {
        }

        /**
         * @brief Destroy the Scoped Value Rollback object
         * 
         */
        ~ScopedValueRollback() {
            m_variable = m_saved_value;
        }
    
    private:
        T& m_variable;
        T& m_saved_value;
    };
}