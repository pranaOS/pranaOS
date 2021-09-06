/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#define PACKED __attribute__((packed))

#define ALIGN(__align) __attribute((aligned(__align)))

#define UNUSED(__stuff) (void)(__stuff)

// FIXME: add more macros 