/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Forward.h>

namespace Base {


template<typename Container, typename ValueType>
class SimpleIteration {
public:
    friend Container;

    constexpr bool is_end() const { return m_index == SimpleIteration::end(m_container).m_index; }
    constexpr size_t index() const { return m_index; }

    constexpr bool operator==(SimpleIteration other) const { return m_index == other.m_index; }
    constexpr bool operator!=(SimpleIteration other) const { return m_index != other.m_index; }
    constexpr bool operator<(SimpleIteration other) const { return m_index < other.m_index; }
    constexpr bool operator>(SimpleIteration other) const { return m_index > other.m_index; }
    constexpr bool operator<=(SimpleIteration other) const { return m_index <= other.m_index; }
    constexpr bool operator>=(SimpleIteration other) const { return m_index >= other.m_index; }

    constexpr SimpleIteration operator+(ptrdiff_t delta) const { return SimpleIteration { m_container, m_index + delta }; }
    constexpr SimpleIteration operator-(ptrdiff_t delta) const { return SimpleIteration { m_container, m_index - delta }; }

    constexpr ptrdiff_t operator-(SimpleIteration other) const { return static_cast<ptrdiff_t>(m_index) - other.m_index; }
}

}