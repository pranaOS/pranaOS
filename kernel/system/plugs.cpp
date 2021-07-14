/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#include <assert.h>
#include <pranaos/Plugs.h>
#include <libabi/Result.h>
#include <libsystem/core/Plugs.h>
#include <libsystem/system/System.h>
#include "archs/Arch.h"
#include "system/Streams.h"
#include "system/graphics/EarlyConsole.h"
#include "system/interrupts/Interupts.h"
#include "system/memory/Memory.h"
#include "system/scheduling/Scheduler.h"
#include "system/system/System.h"
#include "system/tasking/Handles.h"
#include "system/tasking/Task-Launchpad.h"
#include "system/tasking/Task-Memory.h"

void __plug_assert_failed(const char *expr, const char *file, const char *function, int line)
{
    system_panic("Assert failed: %s in %s:%s() ln%d!", (char *)expr, (char *)file, (char *)function, line);
    ASSERT_NOT_REACHED();
}
