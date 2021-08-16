/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libjs/Forward.h>
#include <libweb/Forward.h>

namespace Web::Bindings {

JS::Object* wrap(JS::GlobalObject&, DOM::EventTarget&);

}
