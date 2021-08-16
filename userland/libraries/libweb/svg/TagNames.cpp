/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/svg/TagNames.h>

namespace Web::SVG::TagNames {

#define __ENUMERATE_SVG_TAG(name) FlyString name;
ENUMERATE_SVG_TAGS
#undef __ENUMERATE_SVG_TAG

[[gnu::constructor]] static void initialize()
{
    static bool s_initialized = false;
    if (s_initialized)
        return;

#define __ENUMERATE_SVG_TAG(name) name = #name;
    ENUMERATE_SVG_TAGS
#undef __ENUMERATE_SVG_TAG

    s_initialized = true;
}

}
