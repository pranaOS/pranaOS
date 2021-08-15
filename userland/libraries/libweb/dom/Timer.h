/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/Forward.h>
#include <libcore/Forward.h>
#include <libjs/heap/Handle.h>
#include <libjs/runtime/FunctionObject.h>
#include <libweb/Forward.h>

namespace Web::DOM {

class Timer final : public RefCounted<Timer> {
public:
    enum class Type {
        Interval,
        Timeout,
    };

    static NonnullRefPtr<Timer> create_interval(Window&, int milliseconds, JS::FunctionObject&);
    static NonnullRefPtr<Timer> create_timeout(Window&, int milliseconds, JS::FunctionObject&);

    ~Timer();

    i32 id() const { return m_id; }
    Type type() const { return m_type; }

    JS::FunctionObject& callback() { return *m_callback.cell(); }

private:
    Timer(Window&, Type, int ms, JS::FunctionObject&);

    Window& m_window;
    RefPtr<Core::Timer> m_timer;
    Type m_type;
    int m_id { 0 };
    JS::Handle<JS::FunctionObject> m_callback;
};

}
