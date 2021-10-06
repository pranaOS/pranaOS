/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libswift/runtime.h>
#include <stdlib.h>

#define swift_malloc(...) malloc(__VA_ARGS__)
#define swift_realloc(...) realloc(__VA_ARGS__)
#define swift_calloc(...) calloc(__VA_ARGS__)
#define swift_free(...) free(__VA_ARGS__)

id alloc_instance(Class cls);