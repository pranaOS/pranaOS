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
         * @tparam T::*member 
         */
        template<class T, typename Container, IntrusiveListNode<T, Container> T::*member>
        class IntrusiveListRelaxedConst : public IntrusiveList<T, Container, member>
        {
        public:
            /**
             * @return Iterator 
             */
            Iterator begin() const
            {
                return const_cast<IntrusiveListRelaxedConst*>(this)->IntrusiveList;
            }

            /**
             * @return Iterator 
             */
            Iterator end() const
            {
                return Iterator {};
            }
        }
    } // namespace Detail
} // namespace Mods