/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Types.h>

namespace Base::UBSanitizer {

extern bool g_ubsan_is_deadly;

typedef void* ValueHandle;

class SourceLocation {
public:
    const char* filename() const { return m_filename; }
    u32 line() const { return m_line; }
    u32 column() const { return m_column; }

private:
    const char* m_filename;
    u32 m_line;
    u32 m_column;
};


}
