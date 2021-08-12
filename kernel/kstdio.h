/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Types.h>

extern "C" {
void dbgputch(char);
void dbgputstr(const char*, size_t);
void kernelputstr(const char*, size_t);
void kernelcriticalputstr(const char*, size_t);
int snprintf(char* buf, size_t, const char* fmt, ...) __attribute__((format(printf, 3, 4)));
void set_serial_debug(bool on_or_off);
int get_serial_debug();
}
