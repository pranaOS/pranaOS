/**
 * @file iterator.h
 * @author Krisna Pranav
 * @brief Iterator
 * @version 1.0
 * @date 2023-06-28
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/forward.h>

namespace Mods {

    template<typename Container, typename ValueType>
    class SimpleIterator {
    public:
        friend Container;

        /**
         * @brief is_end
         * 
         * @return true 
         * @return false 
         */
        constexpr bool is_end() const { 
            return m_index == SimpleIterator::end(m_container).m_index; 
        }

        /**
         * @brief index
         * 
         * @return constexpr size_t 
         */
        constexpr size_t index() const { 
            return m_index;
        }

        /**
         * @brief operator==
         * 
         * @param other 
         * @return true 
         * @return false 
         */
        constexpr bool operator==(SimpleIterator other) const { 
            return m_index == other.m_index; 
        }

        /**
         * @brief operator!=
         * 
         * @param other 
         * @return true 
         * @return false 
         */
        constexpr bool operator!=(SimpleIterator other) const { 
            return m_index != other.m_index;
        }

        /**
         * @brief operator<
         * 
         * @param other 
         * @return true 
         * @return false 
         */
        constexpr bool operator<(SimpleIterator other) const { 
            return m_index < other.m_index; 
        }

        /**
         * @brief operator>
         * 
         * @param other 
         * @return true 
         * @return false 
         */
        constexpr bool operator>(SimpleIterator other) const { 
            return m_index > other.m_index; 
        }

        /**
         * @brief <=
         * 
         * @param other 
         * @return true 
         * @return false 
         */
        constexpr bool operator<=(SimpleIterator other) const { 
            return m_index <= other.m_index; 
        }

        /**
         * @brief >=
         * 
         * @param other 
         * @return true 
         * @return false 
         */
        constexpr bool operator>=(SimpleIterator other) const { 
            return m_index >= other.m_index; 
        }

        /**
         * @brief +
         * 
         * @param delta 
         * @return constexpr SimpleIterator 
         */
        constexpr SimpleIterator operator+(ptrdiff_t delta) const { 
            return SimpleIterator { m_container, m_index + delta };
        }

        /**
         * @brief -
         * 
         * @param delta 
         * @return constexpr SimpleIterator 
         */
        constexpr SimpleIterator operator-(ptrdiff_t delta) const { 
            return SimpleIterator { m_container, m_index - delta }; 
        }

        /**
         * @brief -
         * 
         * @param other 
         * @return constexpr ptrdiff_t 
         */
        constexpr ptrdiff_t operator-(SimpleIterator other) const { 
            return static_cast<ptrdiff_t>(m_index) - other.m_index; 
        }
        
        /**
         * @brief ++
         * 
         * @return constexpr SimpleIterator 
         */
        constexpr SimpleIterator operator++() {
            ++m_index;
            return *this;
        }

        /**
         * @brief ++
         * 
         * @return constexpr SimpleIterator 
         */
        constexpr SimpleIterator operator++(int) {
            ++m_index;
            return SimpleIterator { m_container, m_index - 1 };
        }

        /**
         * @brief --
         * 
         * @return constexpr SimpleIterator 
         */
        constexpr SimpleIterator operator--() {
            --m_index;
            return *this;
        }

        /**
         * @brief --
         * 
         * @return constexpr SimpleIterator 
         */
        constexpr SimpleIterator operator--(int) {
            --m_index;
            return SimpleIterator { m_container, m_index + 1 };
        }

        /**
         * @brief *
         * 
         * @return ALWAYS_INLINE constexpr const& 
         */
        ALWAYS_INLINE constexpr const ValueType& operator*() const { return m_container[m_index]; }
        ALWAYS_INLINE constexpr ValueType& operator*() { return m_container[m_index]; }

        /**
         * @brief ->
         * 
         * @return constexpr const ValueType* 
         */
        constexpr const ValueType* operator->() const { return &m_container[m_index]; }
        constexpr ValueType* operator->() { return &m_container[m_index]; }

    private:
        /**
         * @brief begin
         * 
         * @param container 
         * @return constexpr SimpleIterator 
         */
        static constexpr SimpleIterator begin(Container& container) { 
            return { container, 0 }; 
        }
        
        /**
         * @brief end
         * 
         * @param container 
         * @return constexpr SimpleIterator 
         */
        static constexpr SimpleIterator end(Container& container) {
            using RawContainerType = typename RemoveCV<Container>::Type;

            if constexpr (IsSame<StringView, RawContainerType>::value || IsSame<String, RawContainerType>::value)
                return { container, container.length() };
            else
                return { container, container.size() };
        }

        /**
         * @brief simpleiterator
         * 
         */
        constexpr SimpleIterator(Container& container, size_t index)
            : m_container(container)
            , m_index(index)
        {
        }

        Container& m_container;
        size_t m_index;
    };

}
