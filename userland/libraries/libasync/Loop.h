/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libio/Handle.h>
#include <libutils/RefCounted.h>
#include <libutils/Vector.h>

namespace Async

{

using AtExitHook = void (*)(void);

struct Notifier;

struct Timer;

struct Invoker;
}