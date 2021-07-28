/*
 * Copyright (c) 2020, nuke123
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <libelf/Image.h>
#include <stddef.h>
#include <stdint.h>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    ELF::Image elf(data, size, false);
    return 0;
}
