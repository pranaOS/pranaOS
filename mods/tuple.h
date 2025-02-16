/**
 * @file tuple.h
 * @author Krisna Pranav
 * @brief tuple
 * @version 6.0
 * @date 2025-02-16
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/stdlibextra.h>
#include <mods/typelist.h>

namespace Mods::Detail
{
    /**
     * @tparam Ts 
     */
    template <typename... Ts>
    struct Tuple
    {
    };

    /**
     * @tparam T 
     */
    template <typename T>
    struct Tuple<T>
    {
        /**
         * @param value
         * 
         */
        Tuple(T&& value)
            requires(!IsSame < T &&, const T& >)
            : value(forward<T>(value))
        {
        }

        /**
         * @brief Construct a new Tuple object
         * 
         * @param value 
         */
        Tuple(const T& value)
            : value(value)
        {
        }

        /**
         * @tparam U 
         * @return U& 
         */
        template <typename U>
        U& get()
        {
            static_assert(IsSame<T, U>, "Invalid tuple access");
            return value;
        }

        /**
         * @tparam U 
         * @return const U& 
         */
        template <typename U>
        const U& get() const
        {
            return const_cast<Tuple<T>&>(*this).get<U>();
        }

        /**
         * @brief Get the with index object
         * 
         * @tparam U 
         * @tparam index 
         * @return U& 
         */
        template <typename U, unsigned index>
        U& get_with_index()
        {
            static_assert(IsSame<T, U> && index == 0, "Invalid tuple access");
            return value;
        }

        /**
         * @brief Get the with index object
         * 
         * @tparam U 
         * @tparam index 
         * @return const U& 
         */
        template <typename U, unsigned index>
        const U& get_with_index() const
        {
            return const_cast<Tuple<T>&>(*this).get_with_index<U, index>();
        }

    private:
        T value;
    };

    /**
     * @tparam T 
     * @tparam TRest 
     */
    template <typename T, typename... TRest>
    struct Tuple<T, TRest...> : Tuple<TRest...>
    {
        /**
         * @brief Construct a new Tuple object
         * 
         * @tparam FirstT 
         * @tparam RestT 
         * @param first 
         * @param rest 
         */
        template <typename FirstT, typename... RestT>
        Tuple(FirstT&& first, RestT&&... rest)
            : Tuple<TRest...>(forward<RestT>(rest)...), value(forward<FirstT>(first))
        {
        }

        /**
         * @brief Construct a new Tuple object
         * 
         * @param first 
         * @param rest 
         */
        Tuple(T&& first, TRest&&... rest)
            : Tuple<TRest...>(move(rest)...), value(move(first))
        {
        }
        
        /**
         * @tparam U 
         * @return U& 
         */
        template <typename U>
        U& get()
        {
            if constexpr(IsSame<T, U>)
                return value;
            else
                return Tuple<TRest...>::template get<U>();
        }

        /**
         * @tparam U 
         * @return const U& 
         */
        template <typename U>
        const U& get() const
        {
            return const_cast<Tuple<T, TRest...>&>(*this).get<U>();
        }

        /**
         * @brief Get the with index object
         * 
         * @tparam U 
         * @tparam index 
         * @return U& 
         */
        template <typename U, unsigned index>
        U& get_with_index()
        {
            if constexpr(IsSame<T, U> && index == 0)
                return value;
            else
                return Tuple<TRest...>::template get_with_index<U, index - 1>();
        }

        /**
         * @brief Get the with index object
         * 
         * @tparam U 
         * @tparam index 
         * @return const U& 
         */
        template <typename U, unsigned index>
        const U& get_with_index() const
        {
            return const_cast<Tuple<T, TRest...>&>(*this).get_with_index<U, index>();
        }

    private:
        T value;
    }; // struct Tuple<T, TRest...> : Tuple<TRest...>
} // namespace Mods::Detail

namespace Mods
{
    /**
     * @tparam Ts 
     */
    template <typename... Ts>
    struct Tuple : Detail::Tuple<Ts...>
    {
        using Types = TypeList<Ts...>;
        using Detail::Tuple<Ts...>::Tuple;
        using Indices = MakeIndexSequence<sizeof...(Ts)>;

        /**
         * @brief Construct a new Tuple object
         * 
         * @param other 
         */
        Tuple(Tuple&& other)
            : Tuple(move(other), Indices())
        {
        }

        /**
         * @brief Construct a new Tuple object
         * 
         * @param other 
         */
        Tuple(Tuple const& other)
            : Tuple(other, Indices())
        {
        }

        /**
         * @brief 
         * 
         * @param other 
         * @return Tuple& 
         */
        Tuple& operator=(Tuple&& other)
        {
            set(move(other), Indices());
            return *this;
        }

        /**
         * @param other 
         * @return Tuple& 
         */
        Tuple& operator=(Tuple const& other)
        {
            set(other, Indices());
            return *this;
        }

        /**
         * @tparam T 
         * @return auto& 
         */
        template <typename T>
        auto& get()
        {
            return Detail::Tuple<Ts...>::template get<T>();
        }

        /**
         * @tparam index 
         * @return auto& 
         */
        template <unsigned index>
        auto& get()
        {
            return Detail::Tuple<Ts...>::template get_with_index<typename Types::template Type<index>, index>();
        }

        /**
         * @tparam T 
         * @return auto& 
         */
        template <typename T>
        auto& get() const
        {
            return Detail::Tuple<Ts...>::template get<T>();
        }

        /**
         * @tparam index 
         * @return auto& 
         */
        template <unsigned index>
        auto& get() const
        {
            return Detail::Tuple<Ts...>::template get_with_index<typename Types::template Type<index>, index>();
        }

        /**
         * @tparam F 
         * @param f 
         * @return auto 
         */
        template <typename F>
        auto apply_as_args(F&& f)
        {
            return apply_as_args(forward<F>(f), Indices());
        }

        /**
         * @tparam F 
         * @param f 
         * @return auto 
         */
        template <typename F>
        auto apply_as_args(F&& f) const
        {
            return apply_as_args(forward<F>(f), Indices());
        }

        /**
         * @return constexpr auto 
         */
        static constexpr auto size()
        {
            return sizeof...(Ts);
        }

    private:
        /**
         * @brief Construct a new Tuple object
         * 
         * @tparam Is 
         * @param other 
         */
        template <unsigned... Is>
        Tuple(Tuple&& other, IndexSequence<Is...>)
            : Detail::Tuple<Ts...>(move(other.get<Is>())...)
        {
        }

        /**
         * @brief Construct a new Tuple object
         * 
         * @tparam Is 
         * @param other 
         */
        template <unsigned... Is>
        Tuple(Tuple const& other, IndexSequence<Is...>)
            : Detail::Tuple<Ts...>(other.get<Is>()...)
        {
        }

        /**
         * @tparam Is 
         * @param other 
         */
        template <unsigned... Is>
        void set(Tuple&& other, IndexSequence<Is...>)
        {
            ((get<Is>() = move(other.get<Is>())), ...);
        }

        /**
         * @tparam Is 
         * @param other 
         */
        template <unsigned... Is>
        void set(Tuple const& other, IndexSequence<Is...>)
        {
            ((get<Is>() = other.get<Is>()), ...);
        }

        /**
         * @tparam F 
         * @tparam Is 
         * @param f 
         * @return auto 
         */
        template <typename F, unsigned... Is>
        auto apply_as_args(F&& f, IndexSequence<Is...>)
        {
            return forward<F>(f)(get<Is>()...);
        }

        /**
         * @tparam F 
         * @tparam Is 
         * @param f 
         * @return auto 
         */
        template <typename F, unsigned... Is>
        auto apply_as_args(F&& f, IndexSequence<Is...>) const
        {
            return forward<F>(f)(get<Is>()...);
        }
    };

    /**
     * @tparam Args 
     */
    template <class... Args>
    Tuple(Args... args) -> Tuple<Args...>;

} // namespace Mods

using Mods::Tuple;