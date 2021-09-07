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
struct Variant
{

private:
    static const size_t data_size = Max<sizeof(Ts)...>::value;
    static const size_t data_align = Max<alignof(Ts)...>::value;

public:
    Variant()
    {
        _index = 0
        new (&_storage) typename First<Ts...>::Type();
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