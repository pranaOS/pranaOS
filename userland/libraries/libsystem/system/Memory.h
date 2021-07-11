/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libabi/Result.h>
#include <libutils/Prelude.h>

JResult memory_alloc(size_t size, uintptr_t *out_address);

JResult memory_free(uintptr_t address);

JResult memory_include(int handle, uintptr_t *out_address, size_t *out_size);

JResult memory_get_handle(uintptr_t address, int *out_handle);