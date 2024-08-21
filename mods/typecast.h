/**
 * @file typecast.h
 * @author Krisna Pranav
 * @brief TypeCast
 * @version 6.0
 * @date 2023-06-28
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "stdlibextra.h"

namespace Mods {

    template<typename OutputType, typename InputType, bool is_base_type = IsBaseOf<OutputType, InputType>::value>
    struct TypeTraits {
        /**
         * @brief has_type
         * 
         * @return true 
         * @return false 
         */
        static bool has_type(InputType&) {
            static_assert(IsVoid<OutputType>::value, "No TypeTraits for this type");
            return false;
        }
    };

    /**
     * @brief has_type
     * 
     * @tparam OutputType 
     * @tparam InputType 
     */
    template<typename OutputType, typename InputType>
    struct TypeTraits<OutputType, InputType, true> {
        static bool has_type(InputType&) { return true; }
    };

    /**
     * @brief is
     * 
     * @tparam OutputType 
     * @tparam InputType 
     * @param input 
     * @return true 
     * @return false 
     */
    template<typename OutputType, typename InputType>
    inline bool is(InputType* input) {
        return input && TypeTraits<const OutputType, const InputType>::has_type(*input);
    }

    /**
     * @brief is
     * 
     * @tparam OutputType 
     * @tparam InputType 
     * @param input 
     * @return true 
     * @return false 
     */
    template<typename OutputType, typename InputType>
    inline bool is(InputType& input) {
        return TypeTraits<const OutputType, const InputType>::has_type(input);
    }

    /**
     * @brief downcast
     * 
     * @tparam OutputType 
     * @tparam InputType 
     * @param input 
     * @return CopyConst<InputType, OutputType>* 
     */
    template<typename OutputType, typename InputType>
    inline CopyConst<InputType, OutputType>* downcast(InputType* input) {
        static_assert(IsBaseOf<InputType, OutputType>::value);
        ASSERT(!input || is<OutputType>(*input));
        return static_cast<CopyConst<InputType, OutputType>*>(input);
    }

    /**
     * @brief downcast
     * 
     * @tparam OutputType 
     * @tparam InputType 
     * @param input 
     * @return CopyConst<InputType, OutputType>& 
     */
    template<typename OutputType, typename InputType>
    inline CopyConst<InputType, OutputType>& downcast(InputType& input) {
        static_assert(IsBaseOf<InputType, OutputType>::value);
        ASSERT(is<OutputType>(input));
        return static_cast<CopyConst<InputType, OutputType>&>(input);
    }

    #define MOD_BEGIN_TYPE_TRAITS(ClassName)                                   \
        namespace Mods {                                                        \
        template<typename InputType>                                          \
        class TypeTraits<const ClassName, InputType, false> {                 \
        public:                                                               \
            static bool has_type(InputType& input) { return is_type(input); } \
                                                                            \
        private:

    #define MOD_END_TYPE_TRAITS() \
        }                        \
        ;                        \
        }

}

// using the mods 
using Mods::downcast;
using Mods::is;
using Mods::TypeTraits;
