#pragma once

#include <libabi/process.h>
#include <libabi/task.h>
#include <libabi/result.h>
#include <libutils/string.h>

int process_this();

const char *process_name();

JResult process_run(const char *command, int *pid, TaskFlags flags);

void NO_RETURN process_abort();

void NO_RETURN process_exit(int code);