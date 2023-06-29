/**
 * @file temporarychange.h
 * @author Krisna Pranav
 * @brief TemporaryChange
 * @version 1.0
 * @date 2023-06-29
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

namespace Mods {
    template<typename T>
    class TemporaryChange {
    public:
        TemporaryChange(T& variable, T value)
            : m_variable(variable)
            , m_old_value(variable)
        {
            m_variable = variable;
        }

    private:
        T& m_variable;
        T m_old_value;
    };
}