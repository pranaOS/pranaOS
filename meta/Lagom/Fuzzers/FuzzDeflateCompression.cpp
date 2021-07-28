/*
 * Copyright (c) 2021, nuke123-sudo
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <libcompress/Deflate.h>
#include <stdio.h>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    auto result = Compress::DeflateCompressor::compress_all(ReadonlyBytes { data, size });
    return result.has_value();
}
