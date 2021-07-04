/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <string.h>
#include "archs/x86/FPU.h"

char fpu_initial_context[512] ALIGNED(16);
char fpu_registers[512] ALIGNED(16);