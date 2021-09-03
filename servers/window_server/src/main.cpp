/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "Components/ControlBar/ControlBar.h"
#include "Components/MenuBar/MenuBar.h"
#include "Components/MenuBar/Widgets/Clock/Clock.h"
#include "Components/Popup/Popup.h"
#include "Compositor.h"
#include "Connection.h"
#include "CursorManager.h"
#include "Devices.h"
#include "ResourceManager.h"
#include "Screen.h"
#include "WindowManager.h"
#include <cstdlib>
#include <libfoundation/EventLoop.h>
#include <new>
#include <sys/socket.h>
#include <unistd.h>

void start_dock()
{
    if (fork()) {
#ifdef TARGET_DESKTOP
        execve("/System/dock", nullptr, nullptr);
#elif TARGET_MOBILE
        execve("/System/homescreen", nullptr, nullptr);
#endif
        std::abort();
    }
}

int main()
{
    nice(-3);
    start_dock();
    auto* event_loop = new LFoundation::EventLoop();
    new WinServer::Screen();
    new WinServer::Connection(socket(PF_LOCAL, 0, 0));
    new WinServer::CursorManager();
    new WinServer::ResourceManager();
    new WinServer::Popup();
    new WinServer::MenuBar();
#ifdef TARGET_MOBILE
    new WinServer::ControlBar();
#endif
    new WinServer::Compositor();
    new WinServer::WindowManager();
    new WinServer::Devices();

    WinServer::MenuBar::the().add_widget<WinServer::Clock>();

    event_loop->run();
    return 0;
}