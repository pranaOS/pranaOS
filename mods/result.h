/**
 * @file result.h
 * @author Krisna Pranav
 * @brief RESULT
 * @version 6.0
 * @date 2023-07-29
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <mods/assertions.h>
#include <mods/optional.h>

namespace Mods
{
    /**
     * @tparam ValueT 
     * @tparam ErrorT 
     */
    template <typename ValueT, typename ErrorT>
    class [[nodiscard]] Result
    {
    public:
        using ValueType = ValueT;
        using ErrorType = ErrorT;

        /**
         * @brief Construct a new Result object
         * 
         * @param res 
         */
        Result(ValueType const& res)
            : m_result(res)
        {
        }

        /**
         * @brief Construct a new Result object
         * 
         * @param res 
         */
        Result(ValueType&& res)
            : m_result(move(res))
        {
        }

        /**
         * @brief Construct a new Result object
         * 
         * @param error 
         */
        Result(ErrorType const& error)
            : m_error(error)
        {
        }

        /**
         * @brief Construct a new Result object
         * 
         * @param error 
         */
        Result(ErrorType&& error)
            : m_error(move(error))
        {
        }

        /**
         * @brief Construct a new Result object
         * 
         * @param other 
         */
        Result(Result&& other) = default;

        /**
         * @brief Construct a new Result object
         * 
         * @param other 
         */
        Result(Result const& other) = default;

        /**
         * @brief Destroy the Result object
         * 
         */
        ~Result() = default;

        /**
         * @return ValueType& 
         */
        ValueType& value()
        {
            return m_result.value();
        }

        /**
         * @return ErrorType& 
         */
        ErrorType& error()
        {
            return m_error.value();
        }

        bool is_error() const
        {
            return m_error.has_value();
        }

        /**
         * @return ValueType 
         */
        ValueType release_value()
        {
            return m_result.release_value();
        }

        /**
         * @return ErrorType 
         */
        ErrorType release_error()
        {
            return m_error.release_value();
        }

    private:
        Optional<ValueType> m_result;
        Optional<ErrorType> m_error;
    }; // class [[nodiscard]] Result

    /**
     * @tparam ErrorT 
     */
    template <typename ErrorT>
    class [[nodiscard]] Result<void, ErrorT>
    {
    public:
        using ValueType = void;
        using ErrorType = ErrorT;

        /**
         * @brief Construct a new Result object
         * 
         * @param error 
         */
        Result(ErrorType const& error)
            : m_error(error)
        {
        }

        /**
         * @brief Construct a new Result object
         * 
         * @param error 
         */
        Result(ErrorType&& error)
            : m_error(move(error))
        {
        }

        /**
         * @brief Construct a new Result object
         * 
         */
        Result() = default;

        /**
         * @brief Construct a new Result object
         * 
         * @param other 
         */
        Result(Result&& other) = default;

        /**
         * @brief Construct a new Result object
         * 
         * @param other 
         */
        Result(Result const& other) = default;

        /**
         * @brief Destroy the Result object
         * 
         */
        ~Result() = default;

        void value() {};
        void release_value() {};

        /**
         * @return ErrorType& 
         */
        ErrorType& error()
        {
            return m_error.value();
        }

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

    private:
        Optional<ErrorType> m_error;
    }; // class [[nodiscard]] Result<void, ErrorT>
} // namespace Mods

using Mods::Result;