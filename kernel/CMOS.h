/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/Types.h>

namespace CMOS {

u8 read(u8 index);
void write(u8 index, u8 data);

}
