#pragma once

#include <libutils/assert.h>
#include <libutils/std.h>
#include <libutils/traits.h>

namespace Utils
{

template <typename... TStack>
struct Dispatch;

template <>
struct Dispatch<>
{
    static void func(int, auto, auto &)
    {
        Assert::unreachable();
    }
};

template <typename... Ts>
struct Visitor : Ts...
{
    using Ts::operator()...;
};

template <typename... Ts>
Visitor(Ts...) -> Visitor<Ts...>;


}