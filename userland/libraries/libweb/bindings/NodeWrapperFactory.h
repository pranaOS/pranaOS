/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libjs/Forward.h>
#include <libweb/Forward.h>

namespace Web {
namespace Bindings {

NodeWrapper* wrap(JS::GlobalObject&, DOM::Node&);

}
}
