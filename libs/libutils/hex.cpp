/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */


/* includes */
#include <libutils/array.h>
#include <libutils/bytebuffer.h>
#include <libutils/hex.h>
#include <libutils/stringutils.h>
#include <libutils/types.h>
#include <string.h>

namespace Utils {


String encode_hex(const ReadonlyBytes input)
{
    StringBuilder output(input.size() * 2);

    for (auto ch : input)
        output.appendff("{:02x}", ch);

    return output.build();
}

}