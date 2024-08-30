/**
 * @file error.h
 * @author Krisna Pranav
 * @brief error
 * @version 6.0
 * @date 2024-08-30
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/optional.h>
#include <mods/try.h>
#include <mods/variant.h>
#include <mods/string_view.h>

#if defined(__pranaos__) && defined(KERNEL)
#    include <libc/errno_codes.h>
#else
#    include <errno.h>
#    include <string.h>
#endif

namespace Mods
{
    class Error 
    {
    public:
        /**
         * @param code 
         * @return Error 
         */
        static Error from_errno(int code) 
        { 
            return Error(code); 
        }

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
        static Error from_string_literal(StringView string_literal) 
        { 
            return Error(string_literal); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_errno() const 
        { 
            return m_code != 0; 
        }


        bool is_syscall() const 
        { 
            return m_syscall; 
        }

        int code() const 
        { 
            return m_code; 
        }

        /**
         * @return StringView 
         */
        StringView string_literal() const 
        { 
            return m_string_literal; 
        }

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
            : m_code(-rc)
            , m_string_literal(syscall_name)
            , m_syscall(true)
        {
        }

        int m_code { 0 };

        StringView m_string_literal;

        bool m_syscall { false };
    }; // class Error 

    /**
     * @tparam T 
     * @tparam ErrorType 
     */
    template<typename T, typename ErrorType>
    class [[nodiscard]] ErrorOr final : public Variant<T, ErrorType> 
    {
    public:
        using Variant<T, ErrorType>::Variant;

        /**
         * @tparam U 
         */
        template<typename U>
        ALWAYS_INLINE ErrorOr(U&& value) requires(!IsSame<RemoveCVReference<U>, ErrorOr<T>>)
            : Variant<T, ErrorType>(forward<U>(value))
        {
        }

    #ifdef __pranaos__
        /**
         * @param code 
         */
        ErrorOr(ErrnoCode code)
            : Variant<T, ErrorType>(Error::from_errno(code))
        {
        }
    #endif

        /**
         * @return T& 
         */
        T& value()
        {
            return this->template get<T>();
        }

        /**
         * @return T const& 
         */
        T const& value() const 
        { 
            return this->template get<T>(); 
        }

        /**
         * @return ErrorType& 
         */
        ErrorType& error() 
        { 
            return this->template get<ErrorType>(); 
        }

        /**
         * @return ErrorType const& 
         */
        ErrorType const& error() const 
        { 
            return this->template get<ErrorType>(); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_error() const 
        { 
            return this->template has<ErrorType>(); 
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
        using Variant<T, ErrorType>::downcast;
    }; // class [[nodiscard]] ErrorOr final : public Variant<T, ErrorType> 

    /**
     * @tparam ErrorType 
     */
    template<typename ErrorType>
    class [[nodiscard]] ErrorOr<void, ErrorType> 
    {
    public:
        /**
         * @brief Construct a new Error Or object
         * 
         * @param error 
         */
        ErrorOr(ErrorType error)
            : m_error(move(error))
        {
        }

    #ifdef __pranaos__
        /**
         * @brief Construct a new Error Or object
         * 
         * @param code 
         */
        ErrorOr(ErrnoCode code)
            : m_error(Error::from_errno(code))
        {
        }
    #endif

        ErrorOr() = default;

        /**
         * @brief Construct a new ErrorOr object
         * 
         * @param other 
         */
        ErrorOr(ErrorOr&& other) = default;

        /**
         * @brief Construct a new ErrorOr object
         * 
         * @param other 
         */
        ErrorOr(ErrorOr const& other) = default;
        ~ErrorOr() = default;

        /**
         * @param other 
         * @return ErrorOr& 
         */
        ErrorOr& operator=(ErrorOr&& other) = default;

        /**
         * @param other 
         * @return ErrorOr& 
         */
        ErrorOr& operator=(ErrorOr const& other) = default;

        /**
         * @return ErrorType& 
         */
        ErrorType& error() 
        { 
            return m_error.value(); 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_error() const 
        { 
            return m_error.has_value(); 
        }

        /**
         * @return ErrorType 
         */
        ErrorType release_error() 
        { 
            return m_error.release_value(); 
        }

        void release_value() { }

    private:
        Optional<ErrorType> m_error;
    }; // class [[nodiscard]] ErrorOr<void, ErrorType> 

} // namespace Mods

using Mods::Error;
using Mods::ErrorOr;