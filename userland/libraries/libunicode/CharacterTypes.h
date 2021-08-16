/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/Forward.h>
#include <base/String.h>
#include <base/Types.h>
#include <libunicode/Forward.h>

namespace Unicode {

u32 to_unicode_lowercase(u32 code_point);
u32 to_unicode_uppercase(u32 code_point);

String to_unicode_lowercase_full(StringView const&);
String to_unicode_uppercase_full(StringView const&);

Optional<GeneralCategory> general_category_from_string(StringView const&);
bool code_point_has_general_category(u32 code_point, GeneralCategory general_category);

Optional<Property> property_from_string(StringView const&);
bool code_point_has_property(u32 code_point, Property property);
bool is_ecma262_property(Property);

Optional<Script> script_from_string(StringView const&);
bool code_point_has_script(u32 code_point, Script script);
bool code_point_has_script_extension(u32 code_point, Script script);

}
