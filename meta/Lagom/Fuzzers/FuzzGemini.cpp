/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/StringView.h>
#include <base/URL.h>
#include <libgemini/Document.h>
#include <stddef.h>
#include <stdint.h>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    auto gemini = StringView(static_cast<const unsigned char*>(data), size);
    Gemini::Document::parse(gemini, {});
    return 0;
}
