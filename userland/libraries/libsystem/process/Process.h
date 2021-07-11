/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libabi/Process.h>
#include <libabi/Task.h>
#include <libabi/Result.h>
#include <libutils/String.h>

int process_this();

const char *process_name();

JResult process_run(const char *command, int *pid, TaskFlags flags);

void NO_RETURN process_abort();

void NO_RETURN process_exit(int code);

JResult process_cancel(int pid);

JResult process_get_directory(char *buffer, size_t size);

JResult process_set_directory(const char *directory);

String process_resolve(String path);

JResult process_sleep(int time);

JResult process_wait(int pid, int *exit_value);