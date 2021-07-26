/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Types.h>
#include <kernel/Forward.h>

namespace Kernel {

class ProcessPagingScope {
public:
    explicit ProcessPagingScope(Process&);
    ~ProcessPagingScope();

private:
    u32 m_previous_cr3 { 0 };
};

}
