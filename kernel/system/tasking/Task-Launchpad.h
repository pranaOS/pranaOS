/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <libabi/Launchpad.h>
#include "system/tasking/Task.h"

JResult task_launch(Task *parent_task, Launchpad *launchpad, int *pid);
JResult task_exec(Task *parent_task, Launchpad *launchpad);