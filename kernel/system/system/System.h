/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <skift/Time.h>
#include <libutils/Prelude.h>
#include <libutils/SourceLocation.h>
#include "system/handover/Handover.h"

void system_main(Handover *handover);
void system_initialize();
void NO_RETURN system_hang();
void NO_RETURN system_stop();
void system_tick();

uint32_t system_get_tick();

ElapsedTime system_get_uptime();