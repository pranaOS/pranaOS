/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include "system/tasking/Task.h"

namespace Kernel
{

void finalize_task(Task *task);

void finalizer_initialize();

}