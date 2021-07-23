/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Format.h>
#include <errno.h>
#include <string.h>

namespace Base {

class OSError {
public:
    explicit OSError(int error)
        : m_error(error)
    {
    }

    int error() const { return m_error; }
    const char* string() const { return strerror(m_error); }

private:
    int m_error { 0 };
};

template<>
struct Formatter<OSError> : Formatter<StringView> {
    void format(FormatBuilder& builder, const OSError& value)
    {
        Formatter<StringView>::format(builder, value.string());
    }
};

}

using Base::OSError;