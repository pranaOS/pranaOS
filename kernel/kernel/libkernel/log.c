/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <drivers/generic/uart.h>
#include <libkernel/libkern.h>
#include <libkernel/lock.h>
#include <libkernel/log.h>
#include <libkernel/stdarg.h>

#ifndef DEBUG_LOCK
#undef lock_acquire
#undef lock_release
#endif

typedef int (*_putch_callback)(char ch, char* buf_base, size_t* written, void* callback_params)

static lock_t _log_lock;
static const char* HEX_alphabet = "0123456789ABCDEF";
static const char* hex_alphabet = "0123456789abcdef";

static int _printf_hex32(unsigned int value, char* base_buf, size_t* written, _putch_callback callback, void* callback_params)
{
    return _printf_hex32_impl(value, hex_alphabet, base_buf, written, callback, callback_params);
}

static int _printf_HEX32(unsigned int value, char* base_buf, size_t* written, _putch_callback callback, void* callback_params)
{
    return _printf_hex32_impl(value, HEX_alphabet, base_buf, written, callback, callback_params);
}

static int _printf_hex64(unsigned long value, char* base_buf, size_t* written, _putch_callback callback, void* callback_params)
{
    return _printf_hex64_impl(value, hex_alphabet, base_buf, written, callback, callback_params);
}

static int _printf_HEX64(unsigned long value, char* base_buf, size_t* written, _putch_callback callback, void* callback_params)
{
    return _printf_hex64_impl(value, HEX_alphabet, base_buf, written, callback, callback_params);
}

static int _printf_u32(unsigned int value, char* base_buf, size_t* written, _putch_callback callback, void* callback_params)
{
    int nxt = 0;
    char tmp_buf[16];

    while (value > 0) {
        tmp_buf[nxt++] = (value % 10) + '0';
        value /= 10;
    }

    if (nxt == 0) {
        callback('0', base_buf, written, callback_params);
    }

    while (nxt) {
        callback(tmp_buf[--nxt], base_buf, written, callback_params);
    }
    return 0;
}

static int _printf_u64(unsigned long value, char* base_buf, size_t* written, _putch_callback callback, void* callback_params)
{
    int nxt = 0;
    char tmp_buf[32];

    while (value > 0) {
        tmp_buf[nxt++] = (value % 10) + '0';
        value /= 10;
    }

    if (nxt == 0) {
        callback('0', base_buf, written, callback_params);
    }

    while (nxt) {
        callback(tmp_buf[--nxt], base_buf, written, callback_params);
    }
    return 0;
}

static int _printf_i32(int value, char* buf, size_t* written, _putch_callback callback, void* callback_params)
{
    if (value < 0) {
        callback('-', buf, written, callback_params);
        value = -value;
    }
    return _printf_u32(value, buf, written, callback, callback_params);
}

static int _printf_i64(long value, char* buf, size_t* written, _putch_callback callback, void* callback_params)
{
    if (value < 0) {
        callback('-', buf, written, callback_params);
        value = -value;
    }
    return _printf_u64(value, buf, written, callback, callback_params);
}

static int _printf_string(const char* value, char* buf, size_t* writen, _putch_callback callback, void* callback_params)
{
    size_t len = strlen(value);
    for (size_t i = 0; i < len; i++) {
        callback(value[i], buf, writen, callback_params);
    }
    return 0;
}