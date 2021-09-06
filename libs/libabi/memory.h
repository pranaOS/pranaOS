/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#define MEMORY_NONE (0)
#define MEMORY_USER (1 << 0)
#define MEMORY_CLEAR (1 << 1)
typedef unsigned int MemoryFlags;