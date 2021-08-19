/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <EP/Forward.h>

namespace EP::UnicodeUtils {

constexpr bool is_unicode_control_code_point(u32 code_point)
{
    return code_point < 0x20 || (code_point >= 0x80 && code_point < 0xa0);
}

Optional<StringView> get_unicode_control_code_point_alias(u32);

}
