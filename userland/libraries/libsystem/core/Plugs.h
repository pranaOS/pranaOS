/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

// includes
#include <libabi/Filesystem.h>
#include <libabi/Handle.h>
#include <libabi/IOCall.h>
#include <libabi/Launchpad.h>
#include <libabi/System.h>
#include <libabi/Time.h>
#include <libio/Seek.h>
#include <libutils/String.h>

TimeStamp __plug_system_get_time();
Tick __plug_system_get_ticks();

int __plug_process_this();

const char *__plug_process_name();

JResult __plug_process_launch(Launchpad *launchpad, int *pid);

void NO_RETURN __plug_process_exit(int code);

JResult __plug_process_cancel(int pid);

String __plug_process_resolve(String raw_path);

JResult __plug_process_get_directory(char *buffer, size_t size);

JResult __plug_process_set_directory(const char *directory);

JResult __plug_process_sleep(int time);

JResult __plug_process_wait(int pid, int *exit_value);