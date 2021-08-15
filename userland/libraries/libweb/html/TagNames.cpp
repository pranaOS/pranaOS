/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/html/TagNames.h>

namespace Web::HTML::TagNames {

#define __ENUMERATE_HTML_TAG(name) FlyString name;
ENUMERATE_HTML_TAGS
#undef __ENUMERATE_HTML_TAG

[[gnu::constructor]] static void initialize()
{
    static bool s_initialized = false;
    if (s_initialized)
        return;

#define __ENUMERATE_HTML_TAG(name) \
    name = #name;
    ENUMERATE_HTML_TAGS
#undef __ENUMERATE_HTML_TAG

    template_ = "template";

    s_initialized = true;
}

}
