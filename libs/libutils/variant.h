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


    template <typename T>
    requires In<T, Ts...>
    Variant(T value)
    {
        _index = IndexOf<T, Ts...>();
        new (&_storage) T(value);
    }

    Variant(const Variant<Ts...> &other)
    {
        _index = other._index;
        Operations::copy(other._index, &other._storage, &_storage);
    }

    Variant(Variant<Ts...> &&other)
    {
        _index = other._index;
        Operations::move(other._index, &other._storage, &_storage);
    }

    ~Variant()
    {
        Operations::destroy(_index, &_storage);
    }

    Variant<Ts...> &operator=(const Variant<Ts...> &other)
    {
        Operations::destroy(_index, &_storage);
        _index = other._index;
        Operations::copy(_index, &_storage, &other._storage);

        return *this;
    }

    Variant<Ts...> &operator=(Variant<Ts...> &&other)
    {
        if (this != &other)
        {
            std::swap(_index, other._index);
            std::swap(_storage, other._storage);
        }

        return *this;
    }

    template <typename T>
    bool is()
    {
        return (_index == IndexOf<T, Ts...>());
    }

    template <typename T, typename... Args>
    void set(Args &&...args)
    {
        Operations::destroy(_index, &_storage);
        new (&_storage) T(std::forward<Args>(args)...);
        _index = IndexOf<T, Ts...>();
    }

    template <typename T>
    T &get()
    {
        Assert::equal(_index, IndexOf<T, Ts...>());
        return *reinterpret_cast<T *>(&_storage);
    }

    template <typename T, typename Callback>
    void with(Callback callback)
    {
        if (is<T>())
        {
            callback(get<T>());
        }
    }

    template <typename TVisitor>
    void visit(TVisitor &&visitor)
    {
        Operations::visit(_index, &_storage, visitor);
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