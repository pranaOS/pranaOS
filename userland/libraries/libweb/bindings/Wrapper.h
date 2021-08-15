/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/NonnullRefPtr.h>
#include <base/Weakable.h>
#include <libjs/runtime/Object.h>
#include <libweb/Forward.h>

namespace Web::Bindings {

class Wrapper
    : public JS::Object
    , public Weakable<Wrapper> {
    JS_OBJECT(Wrapper, JS::Object);

public:
protected:
    explicit Wrapper(Object& prototype)
        : Object(prototype)
    {
    }
};

}
