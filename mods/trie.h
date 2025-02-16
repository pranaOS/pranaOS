/**
 * @file trie.h
 * @author Krisna Pranav
 * @brief Trie
 * @version 6.0
 * @date 2025-02-16
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/concept.h>
#include <mods/forward.h>
#include <mods/hashmap.h>
#include <mods/ownptr.h>
#include <mods/types.h>

namespace Mods
{
    namespace Detail
    {
        /**
         * @tparam TypeA 
         * @tparam Default 
         */
        template<typename TypeA, typename Default>
        struct SubstituteIfVoid
        {
            using Type = TypeA;
        }; // struct SubstituteIfVoid

        /**
         * @tparam Default 
         */
        template<typename Default>
        struct SubstituteIfVoid<void, Default> 
        {
            using Type = Default;
        }; // struct SubstituteIfVoid<void, Default> 

        /**
         * @tparam DeclaredBaseType 
         * @tparam DefaultBaseType 
         */
        template<typename DeclaredBaseType, typename DefaultBaseType>
        class Trie
        {
            using BaseType = typenme SubstituteIfVoid<DeclaredBaseType>;

        }
    } // namespace Detail
} // namespace Mods