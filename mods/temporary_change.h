/**
 * @file temporary_change.h
 * @author Krisna Pranav
 * @brief TemporaryChange
 * @version 1.0
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
        TemporaryChange(T& variable, T value) : m_variable(variable), m_old_value(variable)
        {
            m_variable = value;
        }


    private:
        T& m_variable;
        T m_old_value;
    };
}

using Mods::TemporaryChange;