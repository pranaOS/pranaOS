/**
 * @file concept.h
 * @author Patrick, pranaOS-developers
 * @brief Concept
 * @version 6.0
 * 
 * @copyright Copyright (c) 2021-2024 Patrick, pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "forward.h"
#include "stdlibextra.h"
#include "iteration.h"

namespace Mods::Concepts 
{
    template<typename T>
    concept Integral = IsIntegral<T>;

    template<typename T>
    concept FloatingPoint = IsFloatingPoint<T>;

    template<typename T>
    concept Arithmetic = IsArithmetic<T>;

    template<typename T>
    concept Signed = IsSigned<T>;

    template<typename T>
    concept Unsigned = IsUnsigned<T>;

    template<typename T>
    concept Enum = IsEnum<T>;

    template<typename T, typename U>
    concept SameAs = IsSame<T, U>;

    /**
     * @tparam T 
     * @tparam S 
     */
    template<typename T, template<typename...> typename S>
    concept SpecializationOf = IsSpecializationOf<T, S>;

    template<typename T>
    concept AnyString = Detail::IsConstructible<StringView, T>;

    /**
     * @tparam T 
     * @tparam U 
     */
    template<typename T, typename U>
    concept HashCompatible = IsHashCompatible<Detail::Decay<T>, Detail::Decay<U>>;

    /**
     * @tparam ArrayT 
     * @tparam ContainedT 
     * @tparam SizeT 
     */
    template<typename ArrayT, typename ContainedT, typename SizeT = size_t>
    concept ArrayLike = requires(ArrayT array, SizeT index)
    {
        {
            array[index]
        }
        -> SameAs<RemoveReference<ContainedT>&>;

        {
            array.size()
        }
        -> SameAs<SizeT>;

        {
            array.span()
        }
        -> SameAs<Span<RemoveReference<ContainedT>>>;

        {
            array.data()
        }
        -> SameAs<RemoveReference<ContainedT>*>;
    };

    /**
     * @tparam Func 
     * @tparam Args 
     */
    template<typename Func, typename... Args>
    concept VoidFunction = requires(Func func, Args... args)
    {
        {
            func(args...)
        }
        -> SameAs<void>;
    };

    /**
     * @tparam Func 
     * @tparam Args 
     */
    template<typename Func, typename... Args>
    concept IteratorFunction = requires(Func func, Args... args)
    {
        {
            func(args...)
        }
        -> SameAs<IterationDecision>;
    };

    /**
     * @tparam T 
     * @tparam EndT 
     */
    template<typename T, typename EndT>
    concept IteratorPairWith = requires(T it, EndT end)
    {
        *it;
        { it != end } -> SameAs<bool>;
        ++it;
    };

    /**
     * @tparam T 
     */
    template<typename T>
    concept IterableContainer = requires
    {
        { declval<T>().begin() } -> IteratorPairWith<decltype(declval<T>().end())>;
    };

} // namespace Mods::Concepts

using Mods::Concepts::Arithmetic;
using Mods::Concepts::ArrayLike;
using Mods::Concepts::Enum;
using Mods::Concepts::FloatingPoint;
using Mods::Concepts::Integral;
using Mods::Concepts::IterableContainer;
using Mods::Concepts::IteratorFunction;
using Mods::Concepts::IteratorPairWith;
using Mods::Concepts::SameAs;
using Mods::Concepts::Signed;
using Mods::Concepts::SpecializationOf;
using Mods::Concepts::Unsigned;
using Mods::Concepts::VoidFunction;
