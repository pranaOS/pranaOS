/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/Namespace.h>

namespace Web::Namespace {

#define __ENUMERATE_NAMESPACE(name, namespace_) FlyString name;
ENUMERATE_NAMESPACES
#undef __ENUMERATE_NAMESPACE

[[gnu::constructor]] static void initialize()
{
    static bool s_initialized = false;
    if (s_initialized)
        return;

#define __ENUMERATE_NAMESPACE(name, namespace_) \
    name = namespace_;
    ENUMERATE_NAMESPACES
#undef __ENUMERATE_NAMESPACE

    s_initialized = true;
}

}
