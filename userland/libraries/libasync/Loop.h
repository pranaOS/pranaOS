/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libio/Handle.h>
#include <libutils/RefCounted.h>
#include <libutils/Vector.h>

namespace Async

{

using AtExitHook = void (*)(void);

struct Notifier;

struct Timer;

struct Invoker;

struct Loop : public RefCounted<Loop>
{
private:
    bool _is_running = false;
    int _exit_value = PROCESS_SUCCESS;

    bool _nested_is_running = false;
    int _nested_exit_value = 0;

    Vector<HandlePoll> _polls;

    Vector<Notifier *> _notifiers;
    Vector<Timer *> _timers;
    Vector<Invoker *> _invoker;

    void update_polling_list();
    
    void update_notifier(int id, PollEvent event);

    void update_timers();

    void update_invoker();

    Timeout get_timeout();


};
}