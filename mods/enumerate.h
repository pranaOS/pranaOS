/**
 * @file enumerate.h
 * @author Krisna Pranav
 * @brief enumerate
 * @version 6.0
 * @date 2025-03-20
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/stdlibextras.h>

namespace Mods 
{

    namespace Detail 
    {
        /**
         * @tparam Iterable 
         */
        template<typename Iterable>
        class Enumerator 
        {
            using IteratorType = decltype(declval<Iterable>().begin());
            using ValueType = decltype(*declval<IteratorType>());

            struct Enumeration {
                size_t index { 0 };
                ValueType value;
            }; // struct Enumeration

        public:
            /**
             * @brief Construct a new Enumerator object
             * 
             * @param iterable 
             */
            Enumerator(Iterable&& iterable)
                : m_iterable(forward<Iterable>(iterable))
                , m_iterator(m_iterable.begin())
                , m_end(m_iterable.end())
            {
            }

            /**
             * @return Enumerator const& 
             */
            Enumerator const& begin() const 
            { 
                return *this; 
            }

            /**
             * @return Enumerator const& 
             */
            Enumerator const& end() const 
            { 
                return *this; 
            }

            /**
             * @return Enumeration 
             */
            Enumeration operator*() 
            { 
                return { m_index, *m_iterator }; 
            }

            /**
             * @return Enumeration 
             */
            Enumeration operator*() const 
            { 
                return { m_index, *m_iterator }; 
            }

            /**
             * @return true 
             * @return false 
             */
            bool operator!=(Enumerator const&) const 
            { 
                return m_iterator != m_end; 
            }

            void operator++()
            {
                ++m_index;
                ++m_iterator;
            }

        private:
            Iterable m_iterable;

            size_t m_index { 0 };
            IteratorType m_iterator;
            IteratorType const m_end;
        }; // class Enumerator

    } // namespace Detail

    /**
     * @tparam T 
     * @param range 
     * @return auto 
     */
    template<typename T>
    auto enumerate(T&& range)
    {
        return Detail::Enumerator<T> { forward<T>(range) };
    }

} // namespace Mods

#ifdef USING_MODS_GLOBALLY
using Mods::enumerate;
#endif
