/**
 * @file find.h
 * @author Krisna Pranav
 * @brief find iteration
 * @version 6.0
 * @date 2024-08-23
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/concept.h>
#include <mods/traits.h>
#include <mods/types.h>

namespace Mods
{
    /**
     * @tparam TEndIterator 
     * @tparam TIterator 
     * @tparam TUnaryPredicate 
     * @param first 
     * @param last 
     * @param pred 
     * @return constexpr TIterator 
     */
    template<typename TEndIterator, IteratorPairWith<TEndIterator> TIterator, typename TUnaryPredicate>
    constexpr TIterator find_if(TIterator first, TEndIterator last, TUnaryPredicate&& pred)
    {
        for (; first != last; ++first) {
            if (pred(*first)) {
                return first;
            }
        }
        return last;
    }
    
    /**
     * @tparam TEndIterator 
     * @tparam TIterator 
     * @tparam T 
     * @param first 
     * @param last 
     * @param value 
     * @return constexpr TIterator 
     */
    template<typename TEndIterator, IteratorPairWith<TEndIterator> TIterator, typename T>
    constexpr TIterator find(TIterator first, TEndIterator last, T const& value)
    {
        return find_if(first, last, [&](auto const& v) { return Traits<T>::equals(value, v); });
    }

    /**
     * @tparam TEndIterator 
     * @tparam TIterator 
     * @tparam T 
     */
    template<typename TEndIterator, IteratorPairWith<TEndIterator> TIterator, typename T>
    constexpr size_t find_index(TIterator first, TEndIterator last, T const& value) requires(requires(TIterator it) { it.index(); })
    {
        return find_if(first, last, [&](auto const& v) { return Traits<T>::equals(value, v); }).index();
    }

} // namespace Mods 