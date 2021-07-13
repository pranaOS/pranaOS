/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// incldues
#include <libutils/List.h>
#include "archs/Arch.h"
#include "system/interrupts/Interupts.h"
#include "system/scheduling/Scheduler.h"
#include "system/system/System.h"

static bool scheduler_context_switch = false;
static int scheduler_record[SCHEDULER_RECORD_COUNT] = {};

static Task *running = nullptr;
static Task *idle = nullptr;

static Utils::List<Task *> *blocked_tasks;
static Utils::List<Task *> *running_tasks;