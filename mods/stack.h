/**
 * @file stack.h
 * @author Krisna Pranav
 * @brief Stack
 * @version 6.0
 * @date 2024-11-06
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/vector.h>

namespace Mods
{
    /**
     * @tparam T 
     * @tparam stack_size 
     */
    template <typename T, size_t stack_size>
    class Stack
    {
    public:
        /**
         * @brief Construct a new Stack object
         * 
         */
        Stack() = default;

        /**
         * @brief Destroy the Stack object
         * 
         */
        ~Stack() = default;

        /**
         * @param item 
         * @return true 
         * @return false 
         */
        bool push(const T& item)
        {
            if(m_stack.size() >= stack_size)
                return false;

            m_stack.unchecked_append(item);
            return true;
        }

        /**
         * @param item 
         * @return true 
         * @return false 
         */
        bool push(T&& item)
        {
            if(m_stack.size() >= stack_size)
                return false;

            m_stack.unchecked_append(move(item));
            return true;
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_empty() const
        {
            return m_stack.is_empty();
        }

        /**
         * @return size_t 
         */
        size_t size() const
        {
            return m_stack.size();
        }

        bool pop()
        {
            if(is_empty())
                return false;

            m_stack.resize_and_keep_capacity(m_stack.size() - 1);
            return true;
        }

        /**
         * @return T& 
         */
        T& top()
        {
            return m_stack.last();
        }

        /**
         * @return const T& 
         */
        const T& top() const
        {
            return m_stack.last();
        }

    private:
        Vector<T, stack_size> m_stack;
    }; // class Stack
} // namespace Mods

using Mods::Stack;