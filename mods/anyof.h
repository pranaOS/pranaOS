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

#include <mods/concepts.h>
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
    [[nodiscard]] constexpr bool all_of(
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
    [[nodiscard]] constexpr bool all_of(Container&& container, auto const& predicate)
    {
        return all_of(container.begin(), container.end(), predicate);
    }

} // namespace Mods
 
#if USING_MODS_GLOBALLY
using Mods::all_of;
#endif
