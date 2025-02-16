/**
 * @file typecast.h
 * @author Krisna Pranav
 * @brief TypeCast
 * @version 6.0
 * @date 2023-06-28
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/assertions.h>
#include <mods/platform.h>
#include <mods/stdlibextra.h>

namespace Mods
{
    /**
     * @tparam OutputType 
     * @tparam InputType 
     * @param input 
     * @return ALWAYS_INLINE 
     */
    template <typename OutputType, typename InputType>
    ALWAYS_INLINE bool is(InputType& input)
    {
        if constexpr(requires { input.template fast_is<OutputType>(); })
        {
            return input.template fast_is<OutputType>();
        }
        return dynamic_cast<CopyConst<InputType, OutputType>*>(&input);
    }

    /**
     * @tparam OutputType 
     * @tparam InputType 
     * @param input 
     * @return ALWAYS_INLINE 
     */
    template <typename OutputType, typename InputType>
    ALWAYS_INLINE bool is(InputType* input)
    {
        return input && is<OutputType>(*input);
    }

    /**
     * @tparam OutputType 
     * @tparam InputType 
     * @param input 
     * @return ALWAYS_INLINE* 
     */
    template <typename OutputType, typename InputType>
    ALWAYS_INLINE CopyConst<InputType, OutputType>* verify_cast(InputType* input)
    {
        static_assert(IsBaseOf<InputType, OutputType>);
        VERIFY(!input || is<OutputType>(*input));
        return static_cast<CopyConst<InputType, OutputType>*>(input);
    }

    /**
     * @tparam OutputType 
     * @tparam InputType 
     * @param input 
     * @return ALWAYS_INLINE& 
     */
    template <typename OutputType, typename InputType>
    ALWAYS_INLINE CopyConst<InputType, OutputType>& verify_cast(InputType& input)
    {
        static_assert(IsBaseOf<InputType, OutputType>);
        VERIFY(is<OutputType>(input));
        return static_cast<CopyConst<InputType, OutputType>&>(input);
    }

} // namespace Mods

using Mods::is;
using Mods::verify_cast;
