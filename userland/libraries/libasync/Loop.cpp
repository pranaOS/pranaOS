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


}