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

    private:
        /**
         * @brief begin
         * 
         * @param container 
         * @return constexpr SimpleIterator 
         */
        static constexpr SimpleIterator begin(Container& container) {
            return { continer, 0 };
        };

        /**
         * @brief SimpleIterator
         * 
         */
        constexpr SimpleIterator(Container& container, size_t index) 
            : m_container(container)
            , m_index(index)
        {}

        Container& m_container;
        size_T m_index;
    };
}