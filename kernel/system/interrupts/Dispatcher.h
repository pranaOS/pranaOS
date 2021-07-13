/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include "system/node/Node.h"

typedef void (*DispatcherInteruptHandler)();

void dispatcher_initialize();
void dispatcher_dispatch(int interrupt);
void dispatcher_service();