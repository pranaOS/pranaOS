/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once
#include <libfoundation/Event.h>
#include <libfoundation/EventLoop.h>
#include <libfoundation/EventReceiver.h>
#include <libg/Size.h>
#include <libui/Connection.h>
#include <libui/Window.h>
#include <memory>
#include <sys/types.h>

#define SET_APP_DELEGATE(name)                                 \
    name* MainAppDelegatePtr;                                  \
    extern "C" bool __init_app_delegate(UI::AppDelegate** res) \
    {                                                          \
        MainAppDelegatePtr = new name();                       \
        *res = MainAppDelegatePtr;                             \
        return MainAppDelegatePtr->application();              \
    }

namespace UI {

class AppDelegate {
public:
    AppDelegate() = default;
    virtual ~AppDelegate() = default;

    LG::Size window_size() const
    {
#ifdef TARGET_DESKTOP
        return preferred_desktop_window_size();
#elif TARGET_MOBILE
        return LG::Size(320, 548);
#endif
    }
    virtual LG::Size preferred_desktop_window_size() const { return LG::Size(400, 300); }
    virtual const char* icon_path() const { return "/res/icons/apps/missing.icon"; }
    virtual bool application() { return false; }
    virtual void application_will_terminate() { }

private:
};

} // namespace UI