/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/runtime/FunctionObject.h>
#include <libweb/dom/EventListener.h>

namespace Web::DOM {

JS::FunctionObject& EventListener::function()
{
    VERIFY(m_function.cell());
    return *m_function.cell();
}

}
