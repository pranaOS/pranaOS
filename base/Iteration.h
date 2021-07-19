/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <base/Forward.h>

namespace Base 
{

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

    constexpr SimpleIteration operator++()
    {
        ++m_index;
        return *this;
    }
    constexpr SimpleIteration operator++(int)
    {
        ++m_index;
        return SimpleIteration { m_container, m_index - 1 };
    }

    constexpr SimpleIteration operator--()
    {
        --m_index;
        return *this;
    }
    constexpr SimpleIteration operator--(int)
    {
        --m_index;
        return SimpleIteration { m_container, m_index + 1 };
    }

    ALWAYS_INLINE constexpr const ValueType& operator*() const { return m_container[m_index]; }
    ALWAYS_INLINE constexpr ValueType& operator*() { return m_container[m_index]; }

    constexpr auto operator->() const { return &m_container[m_index]; }
    constexpr auto operator->() { return &m_container[m_index]; }

    SimpleIteration& operator=(const SimpleIteration& other)
    {
        m_index = other.m_index;
        return *this;
    }
    SimpleIteration(const SimpleIteration& obj) = default;

private:
    static constexpr SimpleIteration begin(Container& container) { return { container, 0 }; }
    static constexpr SimpleIteration end(Container& container)
    {
        using RawContainerType = RemoveCV<Container>;

        if constexpr (IsSame<StringView, RawContainerType> || IsSame<String, RawContainerType>)
            return { container, container.length() };
        else
            return { container, container.size() };
    }

    constexpr SimpleIteration(Container& container, size_t index)
        : m_container(container)
        , m_index(index)
    {
    }

    Container& m_container;
    size_t m_index;
};

}