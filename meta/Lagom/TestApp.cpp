/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <libcore/EventLoop.h>
#include <libcore/Timer.h>
#include <stdio.h>

int main(int, char**)
{
    Core::EventLoop event_loop;

    auto timer = Core::Timer::construct(100, [&] {
        dbgln("Timer fired, good-bye! :^)");
        event_loop.quit(0);
    });

    return event_loop.exec();
}
