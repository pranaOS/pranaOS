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

        /**
         * @tparam ValueType 
         * @tparam MetadataT 
         * @tparam ValueTraits 
         * @tparam BaseT 
         */
        template<typename ValueType, typename MetadataT = void, typename ValueTraits = Traits<ValueType>, typename BaseT = void>
        class Trie : public Detail::Trie<BaseT, Trie<ValueType, MetadataT, ValueTraits>, ValueType, MetadataT, ValueTraits> 
        {
        public:
            using DetailTrie = Detail::Trie<BaseT, Trie<ValueType, MetaDataT, ValueType>>;
            using MetadataType = typename DetailTrie::MetadataType;

            /**
             * @brief Construct a new Trie object
             * 
             * @param value 
             * @param metadata 
             */
            Trie(ValueType value, MetadataType metadata)
                : DetailTrie(move(value), move(metadata))
            {}

            /**
             * @brief Construct a new Trie object
             * 
             * @param value 
             */
            explicit Trie(ValueType value)
                : DetailTrie(move(value), Optional<MetadataType> {})
            {}
        }
    } // namespace Detail
} // namespace Mods