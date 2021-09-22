/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <libkern/types.h>
struct thread;

int signal_impl_prepare_stack(struct thread* thread, int signo, uint32_t old_sp, uint32_t magic);
int signal_impl_restore_stack(struct thread* thread, uint32_t* old_sp, uint32_t* magic);