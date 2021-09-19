#pragma once

#include <libutils/iterationdecision.h>
#include <libutils/stdextras.h>

namespace Utils::Concepts {

template <typename T>
concept Integral = IsIntegral<T>;

template <typename T>
concept FloatingPoint = IsFloatingPoint<T>;

template <typename T>
concept Arithmetic = IsArithmetic<T>;


// FIXME: remove clang formats these properly.
// clang-format: off
template <typename Func, typename... Args>
concept VoidFunction = requires(Func func, Args... args)
{
    {
        func(args...)
    }
    -> SameAs<void>;
};

}

using Utils::Concepts::Arithmetic;