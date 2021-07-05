/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <libabi/Result.h>
#include <string.h>
#include <libjson/Json.h>
#include <libmath/MinMax.h>
#include "procfs/ProcessInfo.h"
#include "system/interrupts/Interupts.h"
#include "system/node/Handle.h"
#include "system/scheduling/Scheduler.h"
#include "system/tasking/Task-Memory.h"

FsProcessInfo::FsProcessInfo() : FsNode(J_FILE_TYPE_DEVICE)
{
}

static Iteration serialize)task(Json::Value::Array *list, Task *task)
{
    if (task->id == 0)
    {
        return Iteration::CONTINUE;
    }

    Json::Value::Object task_object{};


}