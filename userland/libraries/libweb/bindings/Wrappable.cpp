/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/bindings/Wrappable.h>
#include <libweb/bindings/Wrapper.h>

namespace Web {
namespace Bindings {

Wrappable::~Wrappable()
{
}

void Wrappable::set_wrapper(Wrapper& wrapper)
{
    VERIFY(!m_wrapper);
    m_wrapper = wrapper.make_weak_ptr();
}

}
}
