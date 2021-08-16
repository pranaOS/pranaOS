/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/String.h>
#include <base/StringView.h>
#include <libimap/QuotedPrintable.h>

extern "C" int LLVMFuzzerTestOneInput(uint8_t const* data, size_t size)
{
    auto quoted_printable_string = StringView(static_cast<unsigned char const*>(data), size);
    IMAP::decode_quoted_printable(quoted_printable_string);
    return 0;
}
