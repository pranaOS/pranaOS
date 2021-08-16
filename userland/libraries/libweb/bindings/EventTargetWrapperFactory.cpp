/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/bindings/EventTargetWrapperFactory.h>
#include <libweb/dom/EventTarget.h>

namespace Web::Bindings {

JS::Object* wrap(JS::GlobalObject& global_object, DOM::EventTarget& target)
{
    return target.create_wrapper(global_object);
}

}
