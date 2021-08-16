/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/FlyString.h>

namespace Web::UIEvents::EventNames {

#define ENUMERATE_UI_EVENTS          \
    __ENUMERATE_UI_EVENT(click)      \
    __ENUMERATE_UI_EVENT(mousedown)  \
    __ENUMERATE_UI_EVENT(mouseenter) \
    __ENUMERATE_UI_EVENT(mouseleave) \
    __ENUMERATE_UI_EVENT(mousemove)  \
    __ENUMERATE_UI_EVENT(mouseout)   \
    __ENUMERATE_UI_EVENT(mouseover)  \
    __ENUMERATE_UI_EVENT(mouseup)    \
    __ENUMERATE_UI_EVENT(resize)

#define __ENUMERATE_UI_EVENT(name) extern FlyString name;
ENUMERATE_UI_EVENTS
#undef __ENUMERATE_UI_EVENT

}
