/**
 * @file intrusivelistrelaxedconst.h
 * @author Krisna Pranav
 * @brief Intrusive List Relaxed Const
 * @version 6.0
 * @date 2025-02-09
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/intrusivelist.h>

namespace Mods
{
    namespace Detail
    {
        /**
         * @tparam T 
         * @tparam Container 
         * @tparam member 
         */
        template <class T, typename Container, IntrusiveListNode<T, Container> T::* member>
        class IntrusiveListRelaxedConst : public IntrusiveList<T, Container, member>
        {
            MOD_MAKE_NONCOPYABLE(IntrusiveListRelaxedConst);
            MOD_MAKE_NONMOVABLE(IntrusiveListRelaxedConst);

        public:
            using IntrusiveList<T, Container, member>::IntrusiveList;
            using Iterator = typename IntrusiveList<T, Container, member>::Iterator;

            /**
             * @return Iterator 
             */
            Iterator begin() const
            {
                return const_cast<IntrusiveListRelaxedConst*>(this)->IntrusiveList<T, Container, member>::begin();
            }

            /**
             * @return Iterator 
             */
            Iterator end() const
            {
                return Iterator{};
            }
        };
    } // namespace Detail

/**
 * @tparam member 
 */
template <auto member>
using IntrusiveListRelaxedConst = Detail::IntrusiveListRelaxedConst<
    decltype(Detail::ExtractIntrusiveListTypes::value(member)),
    decltype(Detail::ExtractIntrusiveListTypes::container(member)),
    member>;

} // namespace Mods

using Mods::IntrusiveListRelaxedConst;
