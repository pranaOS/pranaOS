/**
 * @file concept.h
 * @author Patrick, pranaOS-developers
 * @brief Concept
 * @version 6.0
 * 
 * @copyright Copyright (c) 2021-2025 Patrick, pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/forward.h>
#include <mods/iterationdecision.h>
#include <mods/stdlibextras.h>

namespace Mods::Concepts 
{

    /**
     * @tparam T 
     */
    template<typename T>
    concept Integral = IsIntegral<T>;

    /**
     * @tparam T 
     */
    template<typename T>
    concept FloatingPoint = IsFloatingPoint<T>;

    /**
     * @tparam T 
     */
    template<typename T>
    concept Fundamental = IsFundamental<T>;

    /**
     * @tparam T 
     */
    template<typename T>
    concept Arithmetic = IsArithmetic<T>;

    /**
     * @tparam T 
     */
    template<typename T>
    concept Signed = IsSigned<T>;

    /**
     * @tparam T 
     */
    template<typename T>
    concept Unsigned = IsUnsigned<T>;

    /**
     * @tparam T 
     */
    template<typename T>
    concept Enum = IsEnum<T>;

    /**
     * @tparam T 
     * @tparam U 
     */
    template<typename T, typename U>
    concept SameAs = IsSame<T, U>;

    /**
     * @tparam From 
     * @tparam To 
     */
    template<class From, class To>
    concept ConvertibleTo = IsConvertible<From, To>;

    /**
     * @tparam U 
     * @tparam Ts 
     */
    template<typename U, typename... Ts>
    concept OneOf = IsOneOf<U, Ts...>;

    /**
     * @tparam U 
     * @tparam Ts 
     */
    template<typename U, typename... Ts>
    concept OneOfIgnoringCV = IsOneOfIgnoringCV<U, Ts...>;

    /**
     * @tparam T 
     * @tparam S 
     */
    template<typename T, template<typename...> typename S>
    concept SpecializationOf = IsSpecializationOf<T, S>;

    /**
     * @tparam T 
     * @tparam S 
     */
    template<typename T, typename S>
    concept DerivedFrom = IsBaseOf<S, T>;

    /**
     * @tparam T 
     */
    template<typename T>
    concept AnyString = IsConstructible<StringView, RemoveCVReference<T> const&>;

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
    concept ArrayLike = requires(ArrayT array, SizeT index) {
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
     * @tparam ArrayT 
     * @tparam ContainedT 
     * @tparam SizeT 
     */
    template<typename ArrayT, typename ContainedT, typename SizeT = size_t>
    concept Indexable = requires(ArrayT array, SizeT index) {
        {
            array[index]
        }
        -> OneOf<RemoveReference<ContainedT>&, RemoveReference<ContainedT>>;
    };

    /**
     * @tparam Func 
     * @tparam Args 
     */
    template<typename Func, typename... Args>
    concept VoidFunction = requires(Func func, Args... args) {
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
    concept IteratorFunction = requires(Func func, Args... args) {
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
    concept IteratorPairWith = requires(T it, EndT end) {
        *it;
        {
            it != end
        } -> SameAs<bool>;
        ++it;
    };

    /**
     * @tparam T 
     */
    template<typename T>
    concept IterableContainer = requires {
        {
            declval<T>().begin()
        } -> IteratorPairWith<decltype(declval<T>().end())>;
    };

    /**
     * @tparam Func 
     * @tparam Args 
     */
    template<typename Func, typename... Args>
    concept FallibleFunction = requires(Func&& func, Args&&... args) {
        func(forward<Args>(args)...).is_error();
        func(forward<Args>(args)...).release_error();
        func(forward<Args>(args)...).release_value();
    };

} // namespace Mods::Concepts

namespace Mods::Detail 
{

    /**
     * @tparam T 
     * @tparam Out 
     * @tparam Args 
     */
    template<typename T, typename Out, typename... Args>
    inline constexpr bool IsCallableWithArguments = requires(T t) {
        {
            t(declval<Args>()...)
        } -> Concepts::ConvertibleTo<Out>;
    } || requires(T t) {
        {
            t(declval<Args>()...)
        } -> Concepts::SameAs<Out>;
    };

} // namespace Mods::Detail

namespace Mods  
{

    using Detail::IsCallableWithArguments;

} // namespace Mods

namespace Mods::Concepts 
{

    /**
     * @tparam Func 
     * @tparam R 
     * @tparam Args 
     */
    template<typename Func, typename R, typename... Args>
    concept CallableAs = Detail::IsCallableWithArguments<Func, R, Args...>;

} // namespace Mods::Concepts 

#if !USING_MODS_GLOBALLY
namespace Mods {
#endif
using Mods::Concepts::Arithmetic;
using Mods::Concepts::ArrayLike;
using Mods::Concepts::CallableAs;
using Mods::Concepts::ConvertibleTo;
using Mods::Concepts::DerivedFrom;
using Mods::Concepts::Enum;
using Mods::Concepts::FallibleFunction;
using Mods::Concepts::FloatingPoint;
using Mods::Concepts::Fundamental;
using Mods::Concepts::Indexable;
using Mods::Concepts::Integral;
using Mods::Concepts::IterableContainer;
using Mods::Concepts::IteratorFunction;
using Mods::Concepts::IteratorPairWith;
using Mods::Concepts::OneOf;
using Mods::Concepts::OneOfIgnoringCV;
using Mods::Concepts::SameAs;
using Mods::Concepts::Signed;
using Mods::Concepts::SpecializationOf;
using Mods::Concepts::Unsigned;
using Mods::Concepts::VoidFunction;
#if !USING_MODS_GLOBALLY
}
#endif
