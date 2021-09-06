/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once 

#include <drivers/generic/mouse.h>
#include <drivers/x86/ata.h>
#include <libkern/types.h>
#include <platform/x86/idt.h>

bool mouse_install();
