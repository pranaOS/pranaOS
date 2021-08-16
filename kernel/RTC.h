/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <kernel/UnixTypes.h>

namespace RTC {

void initialize();
time_t now();
time_t boot_time();

}
