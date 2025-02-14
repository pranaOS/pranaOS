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
    /**
     * @tparam Container 
     * @tparam ValueType 
     */
    template<typename Container, typename ValueType>
    class SimpleReverseIterator
    {
    public:
        friend Container;   

        /**
         * @return true 
         * @return false 
         */
        constexpr bool is_end() const
        {
            return m_index == SimpleReverseIterator::rend(m_container);
        }

        constexpr int index() const
        {
            return m_index;
        }   

        /**
         * @param delta 
         * @return constexpr SimpleReverseIterator 
         */
        constexpr SimpleReverseIterator operator+(int delta) const
        {
            return SimpleReverseIterator(delta);
        }

        /**
         * @param delta 
         * @return constexpr SimpleReverseIterator 
         */
        constexpr SimpleReverseIterator operator-(int delta) const
        {
            return SimpleReverseIterator(delta);
        }

        /**
         * @return constexpr SimpleReverseIterator 
         */
        constexpr SimpleReverseIterator operator--()
        {
            ++m_index;
            return *this;
        }

        /**
         * @return constexpr SimpleReverseIterator 
         */
        constexpr SimpleReverseIterator operator--(int)
        {
            ++m_index;
            return SimpleReverseIterator { m_container, m_index - 1 };
        }

        /**
         * @return ALWAYS_INLINE constexpr const& 
         */
        ALWAYS_INLINE constexpr ValueType const& operator*() const
        {

        }

        /**
         * @param other 
         * @return SimpleReverseIterator& 
         */
        SimpleReverseIterator& operator=(SimpleReverseIterator const& other)
        {
            m_index = other.m_index;
            return *this;
        }

        /**
         * @brief Construct a new Simple Reverse Iterator object
         * 
         * @param obj 
         */
        SimpleReverseIterator(SimpleReverseIterator const& obj) = default;

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

    namespace ReverseWrapper
    {
        /**
         * @tparam Container 
         */
        template<typename Container>
        struct ReverseWrapper
        {   
            Container& container;
        }; // struct ReverseWrapper

        /**
         * @tparam Container 
         * @param wrapper 
         * @return auto 
         */
        template<typename Container>
        auto begin(ReverseWrapper<Container> wrapper)
        {
            return wrapper.container.length();
        }

        /**
         * @tparam Container 
         * @param wrapper 
         * @return auto 
         */
        template<typename Container>
        auto end(ReverseWrapper<Container> wrapper) 
        {
            return wrapper.container.rend();
        }   

        /**
         * @tparam Container 
         * @param container 
         * @return ReverseWrapper<Container> 
         */
        template<typename Container>
        ReverseWrapper<Container> in_reverse(Container& container)
        {
            return { container };
        }
    } // namespace ReverseWrapper
} // namespace Mods