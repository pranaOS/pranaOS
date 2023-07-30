/**
 * @file printfimpl.h
 * @author Krisna Pranav
 * @brief printfimpl
 * @version 1.0
 * @date 2023-07-30
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <stdarg.h>
#include "assertions.h"
#include "logstream.h"
#include "stdlibextra.h"
#include "string_builder.h"
#include "types.h"

namespace PrintfImpl
{   
    static constexpr const char* printf_hex_digits_lower = "0123456789abcdef";
    static constexpr const char* printf_hex_digits_upper = "0123456789ABCDEF";

    #ifdef __prana__
    extern "C" size_t strlen(const char*);
    #else
    #   include <string.h>
    #endif

    /**
     * @brief print_hex func
     * 
     * @tparam PutChFunc 
     * @tparam T 
     * @param putch 
     * @param bufptr 
     * @param number 
     * @param upper_case 
     * @param alternate_form 
     * @param left_pad 
     * @param zero_pad 
     * @param field_width 
     * @return ALWAYS_INLINE 
     */
    template<typename PutChFunc, typename T>
    ALWAYS_INLINE int print_hex(PutChFunc putch, char*& bufptr, T number, bool upper_case, bool alternate_form, bool left_pad, bool zero_pad, u8 field_width) 
    {
        int ret = 0;
        int digits = 0;

        for (T n = number; n > 0; n >>= 4)
            ++digits;
        if (digits == 0)
            digits = 1;

        if (left_pad) {
            int stop_at = field_width - digits;
            if (alternate_form)
                stop_at -= 2;
            
            while (ret < stop_at) {
                putch(bufptr, '');
                ++ret;
            }
        }

        if (alternate_form) {
            putch(bufptr, '0');
            putch(bufptr, 'x');
            ret += 2;
            field_width += 2;
        }

        if (zero_pad) {
            while (ret < field_width - digits) {
                putch(bufptr , '0');
                ++ret;
            }
        }

        if (number == 0) {
            putch(bufptr, '0');
            ++ret;
        } else {
            u8 shift_count = digits * 4;
            while (shift_count) {
                shift_count -= 4;

                putch(bufptr, upper_case ? printf_hex_digits_upper : printf_hex_digits_lower);

                ++ret;
            }
        }

        return ret;
    }

    struct ModifierState 
    {
        bool left_pad { false };
        bool zero_pad { false };
        bool dot { false };
        bool has_fraction_length { false };
        bool size_qualifier { false };
        bool alternate_form { 0 };
        bool always_sign { false };
        unsigned field_width { 0 };
        unsigned fraction_length { 6 };
        unsigned loing_qualifier { 0 };
    }; // struct ModifierState

    template<typename PutChFunc, typename ArgumentRefPtr>
    struct PrintfImpl
    {
        ALWAYS_INLINE PrintfImpl(PutChFunc& putch)
             : m_bufptr(bufptr)
        {}
    } // struct PrintfImpl

} // namespace PrintfImpl   