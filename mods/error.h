/**
 * @file error.h
 * @author Krisna Pranav
 * @brief error
 * @version 6.0
 * @date 2024-08-30
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/stringview.h>
#include <mods/try.h>
#include <mods/variant.h>

#if defined(MODS_OS_PRANAOS) && defined(KERNEL)
#    include <errno_codes.h>
#else
#    include <errno.h>
#    include <string.h>
#endif

namespace Mods 
{

    class [[nodiscard]] Error 
    {
    public:
        ALWAYS_INLINE Error(Error&&) = default;
        ALWAYS_INLINE Error& operator=(Error&&) = default;

        /**
         * @param code 
         * @return Error 
         */
        static Error from_errno(int code)
        {
            VERIFY(code != 0);
            return Error(code);
        }

        /**
         * @param string_literal 
         * @param code 
         * @return Error 
         */
        static Error from_string_view_or_print_error_and_return_errno(StringView string_literal, int code);

    #ifndef KERNEL
        /**
         * @param syscall_name 
         * @param rc 
         * @return Error 
         */
        static Error from_syscall(StringView syscall_name, int rc)
        {
            return Error(syscall_name, rc);
        }

        /**
         * @param string_literal 
         * @return Error 
         */
        static Error from_string_view(StringView string_literal) 
        { 
            return Error(string_literal); 
        }

        /**
         * @tparam T 
         * @return Error 
         */
        template<OneOf<ByteString, DeprecatedFlyString, String, FlyString> T>
        static Error from_string_view(T)
        {
            static_assert(DependentFalse<T>, "Error::from_string_view(String) is almost always a use-after-free");
            VERIFY_NOT_REACHED();
        }

    #endif

        /**
         * @brief 
         * 
         * @param error 
         * @return Error 
         */
        static Error copy(Error const& error)
        {
            return Error(error);
        }

    #ifndef KERNEL
        /**
         * @tparam N 
         * @return ALWAYS_INLINE 
         */
        template<size_t N>
        ALWAYS_INLINE static Error from_string_literal(char const (&string_literal)[N])
        {
            return from_string_view(StringView { string_literal, N - 1 });
        }

    #endif

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(Error const& other) const
        {
    #ifdef KERNEL
            return m_code == other.m_code;
    #else
            return m_code == other.m_code && m_string_literal == other.m_string_literal && m_syscall == other.m_syscall;
    #endif
        }

        int code() const 
        { 
            return m_code; 
        }

        bool is_errno() const
        {
            return m_code != 0;
        }
    #ifndef KERNEL
        bool is_syscall() const
        {
            return m_syscall;
        }
        StringView string_literal() const
        {
            return m_string_literal;
        }
    #endif

    protected:
        /**
         * @brief Construct a new Error object
         * 
         * @param code 
         */
        Error(int code)
            : m_code(code)
        {
        }

    private:
    #ifndef KERNEL
        /**
         * @brief Construct a new Error object
         * 
         * @param string_literal 
         */
        Error(StringView string_literal)
            : m_string_literal(string_literal)
        {
        }

        /**
         * @brief Construct a new Error object
         * 
         * @param syscall_name 
         * @param rc 
         */
        Error(StringView syscall_name, int rc)
            : m_string_literal(syscall_name)
            , m_code(-rc)
            , m_syscall(true)
        {
        }
    #endif

        Error(Error const&) = default;
        Error& operator=(Error const&) = default;

    #ifndef KERNEL
        StringView m_string_literal;
    #endif

        int m_code { 0 };

    #ifndef KERNEL
        bool m_syscall { false };
    #endif
    }; // class Error

    /**
     * @tparam T 
     * @tparam E 
     */
    template<typename T, typename E>
    class [[nodiscard]] ErrorOr 
    {
        template<typename U, typename F>
        friend class ErrorOr;

    public:
        using ResultType = T;
        using ErrorType = E;

        ErrorOr()
        requires(IsSame<T, Empty>)
            : m_value_or_error(Empty {})
        {
        }

        ALWAYS_INLINE ErrorOr(ErrorOr&&) = default;
        ALWAYS_INLINE ErrorOr& operator=(ErrorOr&&) = default;

        ErrorOr(ErrorOr const&) = delete;
        ErrorOr& operator=(ErrorOr const&) = delete;

        /**
         * @tparam U 
         */
        template<typename U>
        ALWAYS_INLINE ErrorOr(ErrorOr<U, ErrorType>&& value)
        requires(IsConvertible<U, T>)
            : m_value_or_error(value.m_value_or_error.visit([](U& v) { return Variant<T, ErrorType>(move(v)); }, [](ErrorType& error) { return Variant<T, ErrorType>(move(error)); }))
        {
        }

        /**
         * @tparam U 
         */
        template<typename U>
        ALWAYS_INLINE ErrorOr(U&& value)
        requires(
            requires { T(declval<U>()); } || requires { ErrorType(declval<RemoveCVReference<U>>()); })
            : m_value_or_error(forward<U>(value))
        {
        }

    #ifdef MODS_OS_PRANAOS
        /**
         * @brief Construct a new Error Or object
         * 
         * @param code 
         */
        ErrorOr(ErrnoCode code)
            : m_value_or_error(Error::from_errno(code))
        {
        }
    #endif

        /**
         * @return T& 
         */
        T& value()
        {
            return m_value_or_error.template get<T>();
        }

        /**
         * @return T const& 
         */
        T const& value() const 
        { 
            return m_value_or_error.template get<T>(); 
        }

        /**
         * @return ErrorType& 
         */
        ErrorType& error() 
        { 
            return m_value_or_error.template get<ErrorType>(); 
        }

        /**
         * @return ErrorType const& 
         */
        ErrorType const& error() const 
        { 
            return m_value_or_error.template get<ErrorType>(); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_error() const 
        { 
            return m_value_or_error.template has<ErrorType>(); 
        }

        /**
         * @return T 
         */
        T release_value() 
        { 
            return move(value()); 
        }

        /**
         * @return ErrorType 
         */
        ErrorType release_error() 
        { 
            return move(error()); 
        }

        /**
         * @return T 
         */
        T release_value_but_fixme_should_propagate_errors()
        {
            VERIFY(!is_error());
            return release_value();
        }

    private:
        Variant<T, ErrorType> m_value_or_error;
    }; // class ErrorOr

    /**
     * @tparam ErrorType 
     */
    template<typename ErrorType>
    class [[nodiscard]] ErrorOr<void, ErrorType> : public ErrorOr<Empty, ErrorType> 
    {
    public:
        using ResultType = void;
        using ErrorOr<Empty, ErrorType>::ErrorOr;
    }; // class [[nodiscard]] ErrorOr<void, ErrorType> : public ErrorOr<Empty, ErrorType> 

} // namespace Mods

#if USING_MODS_GLOBALLY
using Mods::Error;
using Mods::ErrorOr;
#endif
