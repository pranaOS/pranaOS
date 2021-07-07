/*
 * Copyright (c) 2021, nuke123-sudo
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <libutils/Prelude.h>

int early_console_width();
int early_console_height();

void early_console_disable();
void early_console_enable();

size_t early_console_write(const void *buffer, size_t size);
