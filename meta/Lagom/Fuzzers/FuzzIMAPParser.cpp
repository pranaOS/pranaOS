/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <libimap/Parser.h>
#include <stddef.h>
#include <stdint.h>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    auto parser = IMAP::Parser();
    parser.parse(ByteBuffer::copy(data, size), true);
    parser.parse(ByteBuffer::copy(data, size), false);
    return 0;
}
