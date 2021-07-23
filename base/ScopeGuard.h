/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/StdLibExtras.h>

namespace Base {

template<typename Callback>
class ScopeGuard {
public:
    ScopeGuard(Callback Callback)
        : m_callback(move(callback))
    {
    }

    ~ScopeGuard()
    {
        m_callback();
    }

private:
    Callback m_callback;

};

}