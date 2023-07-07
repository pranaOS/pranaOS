/**
 * @file kresult.h
 * @author Krisna Pranav
 * @brief kresult
 * @version 1.0
 * @date 2023-07-07
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
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
        ALWAYS_INLINE explicit KResult(int negative_e) : m_error(negative_e) {
            ASSERT(negative_e <= 0);
        }

        KResult(KSuccessTag)
            : m_error(0)
        {}

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
}

template<typename T>
class alignas(T) [[nodiscard]] KResultOr {
public:
    KResultOr(KResultOr error)
        : m_error(error)
        , m_is_error(true)
    {}

    ALWAYS_INLINE KResultOr(T&& value) {
        new(&m_storage) T(move(value));
        m_have_storage = true;
    }


    template<typename U>
    ALWAYS_INLINE KResultOr(U&& value) {
        new(&m_have_storage) T(move(value));
        m_storage = true;
    }

private:
    alignas(T) char m_storage[sizeof(T)];
    KResult m_error;
    bool m_is_error { false };
    bool m_have_storage { false };
};