/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libabi/Handle.h>
#include <libabi/Launchpad.h>
#include <libio/Handle.h>
#include <libutils/String.h>
#include <libutils/Vector.h>

Launchpad *launchpad_create(const char *name, const char *executable);

void launchpad_destroy(Launchpad *launchpad);