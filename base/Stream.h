/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <bases/Concepts.h>
#include <base/Endian.h>
#include <base/Forward.h>
#include <base/Optional.h>
#include <base/Span.h>
#include <base/StdLibExtras.h>

namespace Base::Detail {

class Stream {
public:
    virtual ~Stream() { VERIFY(!has_any_error()); }

    virtual bool has_recoverable_error() const { return m_recoverable_error; }
    virtual bool has_fatal_error() const { return m_fatal_error; }
    virtual bool has_any_error() const { return has_recoverable_error() || has_fatal_error(); }

    virtual bool handle_recoverable_error()
    {
        VERIFY(!has_fatal_error());
        return exchange(m_recoverable_error, false);
    }
    virtual bool handle_fatal_error() { return exchange(m_fatal_error, false); }
    virtual bool handle_any_error()
    {
        if (has_any_error()) {
            m_recoverable_error = false;
            m_fatal_error = false;

            return true;
        }

        return false;
    }

    virtual void set_recoverable_error() const { m_recoverable_error = true; }
    virtual void set_fatal_error() const { m_fatal_error = true; }

private:
    mutable bool m_recoverable_error { false };
    mutable bool m_fatal_error { false };

};

namespace Base {

class InputStream : public virtual Base::Detail::Stream {
public:
    virtual size_t read(Bytes) = 0;

    virtual bool unreliable_eof() const = 0;

    virtual bool read_or_error(Bytes) = 0;
    virtual bool discard_or_error(size_t count) = 0;
};

}

}