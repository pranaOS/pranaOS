/**
 * @file iterator.h
 * @author Krisna Pranav
 * @brief Iterator
 * @version 6.0
 * @date 2023-06-28
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "forward.h"

namespace Mods
{
    /**
     * @tparam Container 
     * @tparam ValueType 
     */
    template<typename Container, typename ValueType>
    class SimpleIterator 
    {
    public:
        friend Container;

        /**
         * @return true 
         * @return false 
         */
        constexpr bool is_end() const 
        { 
            return m_index == SimpleIterator::end(m_container).m_index; 
        }

        /**
         * @return constexpr size_t 
         */
        constexpr size_t index() const 
        { 
            return m_index; 
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        constexpr bool operator==(SimpleIterator other) const 
        { 
            return m_index == other.m_index; 
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        constexpr bool operator!=(SimpleIterator other) const 
        { 
            return m_index != other.m_index; 
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        constexpr bool operator<(SimpleIterator other) const 
        { 
            return m_index < other.m_index; 
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        constexpr bool operator>(SimpleIterator other) const 
        { 
            return m_index > other.m_index; 
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        constexpr bool operator<=(SimpleIterator other) const 
        { 
            return m_index <= other.m_index; 
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        constexpr bool operator>=(SimpleIterator other) const 
        { 
            return m_index >= other.m_index; 
        }

        /**
         * @param delta 
         * @return constexpr SimpleIterator 
         */
        constexpr SimpleIterator operator+(ptrdiff_t delta) const 
        { 
            return SimpleIterator { m_container, m_index + delta }; 
        }

        /**
         * @param delta 
         * @return constexpr SimpleIterator 
         */
        constexpr SimpleIterator operator-(ptrdiff_t delta) const 
        { 
            return SimpleIterator { m_container, m_index - delta }; 
        }

        /**
         * @param other 
         * @return constexpr ptrdiff_t 
         */
        constexpr ptrdiff_t operator-(SimpleIterator other) const 
        { 
            return static_cast<ptrdiff_t>(m_index) - other.m_index; 
        }

        /**

         * @return constexpr SimpleIterator 
         */
        constexpr SimpleIterator operator++()
        {
            ++m_index;
            return *this;
        }

        /**
         * @return constexpr SimpleIterator 
         */
        constexpr SimpleIterator operator++(int)
        {
            ++m_index;
            return SimpleIterator { m_container, m_index - 1 };
        }   

        /**
         * @return constexpr SimpleIterator 
         */
        constexpr SimpleIterator operator--()
        {
            --m_index;
            return *this;
        }

        /**
         * @return constexpr SimpleIterator 
         */
        constexpr SimpleIterator operator--(int)
        {
            --m_index;
            return SimpleIterator { m_container, m_index + 1 };
        }

        /**
         * @return ALWAYS_INLINE constexpr const& 
         */
        ALWAYS_INLINE constexpr ValueType const& operator*() const 
        { 
            return m_container[m_index]; 
        }

        /**
         * @return ALWAYS_INLINE constexpr& 
         */
        ALWAYS_INLINE constexpr ValueType& operator*() 
        { 
            return m_container[m_index]; 
        }

        /**
         * @return ALWAYS_INLINE constexpr const* 
         */
        ALWAYS_INLINE constexpr ValueType const* operator->() const 
        { 
            return &m_container[m_index]; 
        }

        /**
         * @return ALWAYS_INLINE constexpr* 
         */
        ALWAYS_INLINE constexpr ValueType* operator->() 
        { 
            return &m_container[m_index];
        }

        /**
         * @param other 
         * @return SimpleIterator& 
         */
        SimpleIterator& operator=(const SimpleIterator& other)
        {
            m_index = other.m_index;
            return *this;
        }

        /**
         * @param obj 
         */
        SimpleIterator(const SimpleIterator& obj) = default;

    private:

        /**
         * @param container 
         * @return constexpr SimpleIterator 
         */
        static constexpr SimpleIterator begin(Container& container) { return { container, 0 }; }

        /**
         * @param container 
         * @return constexpr SimpleIterator 
         */
        static constexpr SimpleIterator end(Container& container)
        {
            using RawContainerType = RemoveCV<Container>;

            if constexpr (IsSame<StringView, RawContainerType> || IsSame<String, RawContainerType>)
                return { container, container.length() };
            else
                return { container, container.size() };
        }

        constexpr SimpleIterator(Container& container, size_t index)
            : m_container(container)
            , m_index(index)
        { }

        Container& m_container;
        size_t m_index;
    }; // class SimpleIterator

} // namespace Mods