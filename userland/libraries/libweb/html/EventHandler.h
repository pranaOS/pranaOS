/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/String.h>
#include <libjs/heap/Handle.h>
#include <libjs/runtime/FunctionObject.h>

namespace Web::HTML {

struct EventHandler {
    EventHandler()
    {
    }

    EventHandler(String s)
        : string(move(s))
    {
    }

    EventHandler(JS::Handle<JS::FunctionObject> c)
        : callback(move(c))
    {
    }

    String string;
    JS::Handle<JS::FunctionObject> callback;
};

}
