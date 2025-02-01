/**
 * @file all.h
 * @author Krisna Pranav
 * @brief All Iter
 * @version 6.0
 * @date 2023-06-27
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
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
    constexpr bool all_of(
        TIterator const& begin,
        TEndIterator const& end,
        auto const& predicate)
    {
        constexpr auto negated_predicate = [](auto const& pred) {
            return [&](auto const& elem) { return !pred(elem); };
        };
        return !(find_if(begin, end, negated_predicate(predicate)) != end);
    }

    /**
     * @tparam Container 
     * @param container 
     * @param predicate 
     * @return true 
     * @return false 
     */
    template<IterableContainer Container>
    constexpr bool all_of(Container&& container, auto const& predicate)
    {
        return all_of(container.begin(), container.end(), predicate);
    }
} // namespace Mods

using Mods::all_of;