/**
 * @file bigintbase.h
 * @author Krisna Pranav
 * @brief big int base
 * @version 6.0
 * @date 2025-03-13
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/builtinwrappers.h>
#include <mods/span.h>
#include <mods/stdlibextras.h>
#include <mods/types.h>

namespace Mods
{

    namespace Detail
    {
        /**
         * @tparam T 
         */
        template<typename T>
        struct DoubleWordHelper;

        template<>
        struct DoubleWordHelper<u32>
        {
            using Type = u64;
            using SignedType = i64;
        }; // struct DoubleWordHelper<u32>

        /**
         * @tparam T 
         */
        template<typename T>
        using DoubleWord = typename DoubleWordHelper<T>::Type;

        /**
         * @tparam T 
         */
        template<typeame T>
        using SignedDoubleWord = typename DoubleWordHelper<T>::SignedType;
    } // namespace Detail

} // namespace Mods