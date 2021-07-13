/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include "system/tasking/Task.h"

void fpu_initialize();
void fpu_save_context(Task *task);
void fpu_load_context(Task *task);
void fpu_init_context(Task *task);
