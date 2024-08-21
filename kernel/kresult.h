/**
 * @file kresult.h
 * @author Krisna Pranav
 * @brief kresult
 * @version 6.0
 * @date 2023-07-07
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/assertions.h>
#include <mods/platform.h>
#include <libraries/libc/errno_codes.h>


namespace Kernel {

    enum KSuccessTag {
        KSuccess
    };

    class [[nodiscard]] KResult {
    public:

        /**
         * @param negative_e 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE explicit KResult(int negative_e) : m_error(negative_e)
        {
            ASSERT(negative_e <= 0);
        }

        KResult(KSuccessTag) : m_error(0)
        { }

        /**
         * @return int 
         */
        operator int() const { 
            return m_error; 
        }

        /**
         * @return int 
         */
        [[nodiscard]] int error() const { 
            return m_error; 
        }

        /**
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool is_success() const { 
            return m_error == 0; 
        }

        /**
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool is_error() const { 
            return !is_success(); 
        }

    private:
        template<typename T>
        friend class KResultOr;
        KResult() {}

        int m_error { 0 };
    };

    template<typename T>
    class alignas(T) [[nodiscard]] KResultOr
    {
    public:

        /**
         * @param error 
         */
        KResultOr(KResult error) : m_error(error) , m_is_error(true)
        { }

        /**
         * @param value 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE KResultOr(T&& value) {
            new (&m_storage) T(move(value));
            m_have_storage = true;
        }

        /**
         * @tparam U 
         * @param value 
         * @return ALWAYS_INLINE 
         */
        template<typename U>
        ALWAYS_INLINE KResultOr(U&& value) {
            new (&m_storage) T(move(value));
            m_have_storage = true;
        }

        /**
         * @param other 
         */
        KResultOr(KResultOr&& other) {
            m_is_error = other.m_is_error;
            if (m_is_error)
                m_error = other.m_error;
            else {
                if (other.m_have_storage) {
                    new (&m_storage) T(move(other.value()));
                    m_have_storage = true;
                    other.value().~T();
                    other.m_have_storage = false;
                }
            }
            other.m_is_error = true;
            other.m_error = KSuccess;
        }

        /**
         * @param other 
         * @return KResultOr& 
         */
        KResultOr& operator=(KResultOr&& other) {
            if (!m_is_error && m_have_storage) {
                value().~T();
                m_have_storage = false;
            }
            m_is_error = other.m_is_error;
            if (m_is_error)
                m_error = other.m_error;
            else {
                if (other.m_have_storage) {
                    new (&m_storage) T(move(other.value()));
                    m_have_storage = true;
                    other.value().~T();
                    other.m_have_storage = false;
                }
            }
            other.m_is_error = true;
            other.m_error = KSuccess;
            return *this;
        }

        /**
         * @brief Destroy the KResultOr object
         * 
         */
        ~KResultOr() {
            if (!m_is_error && m_have_storage)
                value().~T();
        }

        /**
         * @return true 
         * @return false 
         */
        [[nodiscard]] bool is_error() const { 
            return m_is_error; 
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE KResult error() const {
            ASSERT(m_is_error);
            return m_error;
        }

        /**
         * @return KResult 
         */
        KResult result() const { 
            return m_is_error ? KSuccess : m_error; 
        }

        /**
         * @return ALWAYS_INLINE& 
         */
        ALWAYS_INLINE T& value() {
            ASSERT(!m_is_error);
            return *reinterpret_cast<T*>(&m_storage);
        }

        /**
         * @return ALWAYS_INLINE const& 
         */
        ALWAYS_INLINE const T& value() const {
            ASSERT(!m_is_error);
            return *reinterpret_cast<T*>(&m_storage);
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE T release_value() {
            ASSERT(!m_is_error);
            ASSERT(m_have_storage);
            T released_value = *reinterpret_cast<T*>(&m_storage);
            value().~T();
            m_have_storage = false;
            return released_value;
        }

    private:
        /// @brief m_storage
        alignas(T) char m_storage[sizeof(T)];

        /// @brief error
        KResult m_error;
        bool m_is_error { false };
        bool m_have_storage { false };
    };

}
