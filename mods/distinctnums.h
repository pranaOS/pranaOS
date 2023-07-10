/**
 * @file distinctnums.h
 * @author Krisna Pranav
 * @brief distinctnums
 * @version 1.0
 * @date 2023-07-10
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "types.h"

namespace Mods {
    template<typename T, bool Incr, bool Cmp, bool Bool, bool Flags, bool Shift>
    class DistinctNumeric {
        using Self = DistinctNumeric<T, Incr, Cmp, Bool, Flags, Shift>;

    public:
        DistinctNumeric(T value) : m_value(value) {}

        const T& value() const {
            return m_value;
        }

        bool operator==(const Self& other) const {
            return this->m_value == other.m_value;
        }

        bool operator!=(const Self& other) const {
            return this->m_value != other.m_value;
        }

        Self& operator++() {
            static_assert(Incr, "++a is only available");
            this->m_value += 1;
            return *this;
        }

        Self operator--(int) {
            static_assert(Incr, "a--");
            Self ret = this->m_value;
            this->m_value -= 1;
            return ret;
        }

        bool operator>(const Self& other) const {
            static_assert(Cmp, "a>b");
            return this->m_value > other.m_value;
        }
        

    private:
        T m_value;
    };
}

using Mods::DistinctNumeric;