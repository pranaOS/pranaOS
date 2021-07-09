/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <libutils/Func.h>
#include <libutils/OwnPtr.h>
#include <libutils/Vector.h>

namespace Async
{

template <typename T>
struct Observable
{
public:
    struct _Observer
    {
        Observable<T> *_observable;
        Func<void(T &)> _callback;

        NONCOPYABLE(_Observer);
        NONMOVABLE(_Observer);

        _Observer(Observable<T> *observable, Func<void(T &)> callback)
            : _observable(observable),
              _callback(callback)
        {
            _observable->register_observer(this);
        }

        ~_Observer()
        {
            if (_observable)
            {
                _observable->unregister_observer(this);
            }
        }

        void detach()
        {
            _observable = nullptr;
            _callback = nullptr;
        }

        void notify(T &subject)
        {
            if (_callback)
            {
                _callback(subject);
            }
        }
    };

}