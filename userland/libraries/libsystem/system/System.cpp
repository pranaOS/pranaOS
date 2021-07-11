/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


// includes
#include <libsystem/core/Plugs.h>

Tick system_get_ticks()
{
    return __plug_system_get_ticks();
}