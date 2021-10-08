/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libutils/stdextras.h>

namespace Utils {

class ScopeGuard {
public:
    ScopeGuard(Callback callback)
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

using Utils::ScopeGuard;