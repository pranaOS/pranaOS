/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libasync/Source.h>
#include <libutils/Func.h>

namespace Async
{

struct Invoker : public Source
{
private:
    bool _invoke_later = false;
    Func<void()> _callback;
    RefPtr<Loop> _eventloop;
    
};

public:
    Invoker(Func<void()> callback) : _callback(callback)
    {
        loop().register_invoker(this);
    }

    ~Invoker()
    {
        loop().unregister_invoker(this);
    }

    [[nodiscard]] bool should_be_invoke_later()
    {
        return _invoker_later;
    }


}