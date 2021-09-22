/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <libkern/c_attrs.h>
#include <libkern/kassert.h>
#include <libkern/types.h>

#define atomic_add(x, val) (__atomic_add_fetch(x, val, __ATOMIC_SEQ_CST))
#define atomic_store(x, val) (__atomic_store_n(x, val, __ATOMIC_SEQ_CST))
#define atomic_load(x) (__atomic_load_n(x, __ATOMIC_SEQ_CST))