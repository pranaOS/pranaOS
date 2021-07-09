/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <libasync/Invoker.h>
#include <libasync/Loop.h>
#include <libasync/Notifier.h>
#include <libasync/Timer.h>
#include <libsystem/system/System.h>
#include <libutils/Assert.h>
#include <libutils/Vector.h>


namespace Async
{

static RefPtr<Loop> _instance = nullptr;

RefPtr<Loop> Loop::the()
{
    if (_instance == nullptr)
    {
        _instance = make<Loop>();
    }

    return _instance;
}

void Loop::update_notifier(int id, PollEvent event)
{
    for (size_t i = 0; i < _notifiers.count(); i++)
    {
        if (_notifiers[i]->handle()->id() == id &&
            _notifiers[i]->events() & event)
        {
            _notifiers[i]->invoke();
        }
    }
}

void Loop::update_polling_list()
{
    _polls.clear();

    for (Notifier *notifier : _notifiers)
    {
        _polls.push_back({
            notifier->handle()->id(),
            notifier->events(),
            0,
        });
    }
}

void Loop::register_notifer(Notifier *notifer)
{
    _notifiers.push_back(notifer);

    update_polling_list();
}

void Loop::unregister_notifier(Notifer *notifier)
{
    _notifers.remove_all_value(notifer);

    update_polling_list();
}

void Loop::register_timer(Timer *timer)
{
    _timers.push_back(timer);
}

void Loop::unregister_timer(Timer *timer)
{
    _timers.remove_value(timer);
}

void Loop::update_timers()
{
    TimeStamp current_fire = system_get_ticks();

    auto timers_list_copy = _timers;

    timers_list_copy.foreach([&](auto timer) {
        if (timer->running() && timer->scheduled() <= current_fire)
        {
            timer->trigger();
            timer->schedule(current_fire + timer->interval());
        }

        return Iteration::CONTINUE;
    });
}

void Loop::registers_invoker(Invoker *invoker)
{
    _invoker.push_back(invoker);
}

void Loop::unregister_invoker(Invoker *invoker)
{
    _invoker.remove_value(invoker);
}

void Loop::update_invoker()
{
    _invoker.foreach([](Invoker *invoker) {
        if (invoker->should_be_invoke_later())
        {
            invoker->invoke();
        }

        return Iteration::CONTINUE;
    });
}

}