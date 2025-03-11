/**
 * @file any.h
 * @author Krisna Pranav
 * @brief any
 * @version 6.0
 * @date 2024-08-24
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/concept.h>
#include <mods/find.h>
#include <mods/iterator.h>

namespace Mods 
{
    /**
     * @tparam TEndIterator 
     * @tparam TIterator 
     * @param begin 
     * @param end 
     * @param predicate 
     * @return true 
     * @return false 
     */
    template<typename TEndIterator, IteratorPairWith<TEndIterator> TIterator>
    constexpr bool any_of(
        TIterator const& begin,
        TEndIterator const& end,
        auto const& predicate)
    {
        return find_if(begin, end, predicate) != end;
    }

    /**
     * @tparam Container 
     * @param container 
     * @param predicate 
     * @return true 
     * @return false 
     */
    template<IterableContainer Container>
    constexpr bool any_of(Container&& container, auto const& predicate)
    {
        return any_of(container.begin(), container.end(), predicate);
    }

}

using Mods::any_of;