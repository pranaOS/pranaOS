/**
 * @file intrusivedetails.h
 * @author Krisna Pranav
 * @brief 
 * @version 0.1
 * @date 2024-10-20
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/nonnullrefptr.h>

namespace Mods::Detail 
{

    /**
     * @tparam T 
     * @tparam Container 
     */
    template<typename T, typename Container>
    struct SubstituteIntrusiveContainerType 
    {
        using Type = Container;
    };

    /**
     * @tparam T 
     */
    template<typename T>
    struct SubstituteIntrusiveContainerType<T, NonnullRefPtr<T>> 
    {
        using Type = RefPtr<T>;
    };

    /**
     * @tparam Container 
     * @tparam _IsRaw 
     */
    template<typename Container, bool _IsRaw>
    struct SelfReferenceIfNeeded 
    {
        Container reference = nullptr;
    };
    
    /**
     * @tparam Container 
     */
    template<typename Container>
    struct SelfReferenceIfNeeded<Container, true> 
    {
    };

} // namespace Mods::Details