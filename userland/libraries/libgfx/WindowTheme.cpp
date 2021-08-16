/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <libgfx/ClassicWindowTheme.h>
#include <libgfx/WindowTheme.h>

namespace Gfx {

WindowTheme& WindowTheme::current()
{
    static ClassicWindowTheme theme;
    return theme;
}

WindowTheme::~WindowTheme()
{
}

}
