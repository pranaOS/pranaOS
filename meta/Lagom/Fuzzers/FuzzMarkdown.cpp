/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/OwnPtr.h>
#include <base/StringView.h>
#include <libmarkdown/Document.h>
#include <stddef.h>
#include <stdint.h>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    auto markdown = StringView(static_cast<const unsigned char*>(data), size);
    Markdown::Document::parse(markdown);
    return 0;
}