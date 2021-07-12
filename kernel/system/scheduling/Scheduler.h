/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include "system/tasking/Task.h"

#define SCHEDULER_RECORD_COUNT 1000

void scheduler_initialize();

void scheduler_did_create_idle_task(Task *task);

void scheduler_did_change_task_state(Task *task, TaskState oldstate, TaskState newstate);

bool scheduler_is_context_switch();

Task *scheduler_running();

uintptr_t schedule(uinptr_t current_stack_pointer);
