/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/StringView.h>
#include <shell/Shell.h>
#include <stddef.h>
#include <stdint.h>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    auto source = StringView(static_cast<const unsigned char*>(data), size);
    Shell::Parser parser(source);
    parser.parse();
    return 0;
}
