/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <base/IterationDecision.h>
#include <base/StdLibExtras.h>

namespace Base::Concepts {

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

template<typename Func, typename... Args>
concept VoidFunction = requires(Func func, Args... args)
{
    {
        func(args...)
    }
    -> SameAs<void>;
};

template<typename Func, typename... Args>
concept IteratorFunction = requires(Func func, Args... args)
{
    {
        func(args...)
    }
    -> SameAs<IterationDecision>;
};
}

using Base::Concepts::Arithmetic;
using Base::Concepts::Enum;
using Base::Concepts::FloatingPoint;
using Base::Concepts::Integral;
using Base::Concepts::IteratorFunction;
using Base::Concepts::Signed;
using Base::Concepts::Unsigned;
using Base::Concepts::VoidFunction;