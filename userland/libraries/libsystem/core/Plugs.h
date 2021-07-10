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

