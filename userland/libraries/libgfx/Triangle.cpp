/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#include <base/String.h>
#include <libgfx/Triangle.h>

namespace Gfx {

String Triangle::to_string() const
{
    return String::formatted("({},{},{})", m_a, m_b, m_c);
}

}
