/**
 * @file find.h
 * @author Krisna Pranav
 * @brief find iteration
 * @version 6.0
 * @date 2024-08-23
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/concepts.h>
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
    [[nodiscard]] constexpr TIterator find_if(TIterator first, TEndIterator last, TUnaryPredicate&& pred)
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
     * @tparam V 
     * @param first 
     * @param last 
     * @param value 
     * @return constexpr TIterator 
     */
    template<typename TEndIterator, IteratorPairWith<TEndIterator> TIterator, typename V>
    [[nodiscard]] constexpr TIterator find(TIterator first, TEndIterator last, V const& value)
    {
        return find_if(first, last, [&]<typename T>(T const& entry) { return Traits<T>::equals(entry, value); });
    }

    /**
     * @tparam TEndIterator 
     * @tparam TIterator 
     * @tparam V 
     */
    template<typename TEndIterator, IteratorPairWith<TEndIterator> TIterator, typename V>
    [[nodiscard]] constexpr size_t find_index(TIterator first, TEndIterator last, V const& value)
    requires(requires(TIterator it) { it.index(); })
    {
        return find_if(first, last, [&]<typename T>(T const& entry) { return Traits<T>::equals(entry, value); }).index();
    }

}

#if USING_MODS_GLOBALLY
using Mods::find;
using Mods::find_if;
using Mods::find_index;
#endif
