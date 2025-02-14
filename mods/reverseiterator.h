/**
 * @file reverseiterator.h
 * @author Krisna Pranav
 * @brief Reverse Iterator
 * @version 6.0
 * @date 2025-02-14
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/forward.h>

namespace Mods
{
    template<typename Container, typename ValueType>
    class SimpleReverseIterator
    {
    private:
        /**
         * @param container 
         * @return constexpr SimpleReverseIterator 
         */
        static constexpr SimpleReverseIterator rbegin(Container& container)
        {
            using RawContainerType = RemoveCV<Container>;

            if constexpr (IsSame<StringView, RawContainerType> || IsSame<String, RawContainerType>)
                return { container, static_cast<int>(container.length()) - 1};
            else
                return { container, static_cast<int>(container.size()) - 1};
        }

        static constexpr SimpleReverseIterator rend(Container& container)
        {
            return { container, -1 };
        }

        constexpr SimpleReverseIterator(Container& container, int index)
            : m_container(container)
            , m_index(index)
        {}

        Container& m_container;
        int m_index { 0 };
    }; // class SimpleReverseIterator

} // namespace Mods