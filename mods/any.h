/**
 * @file any.h
 * @author Krisna Pranav
 * @brief any
 * @version 6.0
 * @date 2024-08-24
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "concept.h"
#include "find.h"
#include "iterator.h"

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
    constexpr bool any_of(TIterator const& begin, TEndIterator const& end, auto const& predicate)
    {
        return find_if(begin, end, predicate) != end;
    }
} // namespace mods 

using Mods::any_of;