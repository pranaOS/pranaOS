/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Noncopyable.h>
#include <base/Span.h>
#include <base/StdLibExtras.h>
#include <base/Stream.h>
#include <base/Types.h>
#include <base/kmalloc.h>

namespace Base {


template<typename StreamType, size_t Size = 4096, typename = void>
class Buffered;

template<typename StreamType, size_t Size>
class Buffered<StreamType, Size, typename EnableIf<IsBaseOf<InputStream, StreamType>>::Type> final : public InputStream {
    Base_MAKE_NONCOPYABLE(Buffered);

public:
    template<typename... Parameters>
    explict Buffered(Parameters&&... parameters)
        : m_stream(forward<Parameters>(parameters)...)
    {
    }

    Buffered(Buffered&& other)
        : m_stream(move(other.m_stream))
    {
        other.buffer().copy_to(buffer());
        m_buffered = exchange(other.m_buffered, 0);
    }

    bool has_recoverable_error() const override { return m_stream.has_recoverable_error(); }
    bool has_fatal_error() const override { return m_stream.has_fatal_error(); }
    bool has_any_error() const override { return m_stream.has_any_error(); }

    bool handle_recoverable_error() override { return m_stream.handle_recoverable_error(); }
    bool handle_fatal_error() override { return m_stream.handle_fatal_error(); }
    bool handle_any_error() override { return m_stream.handle_any_error(); }

    void set_recoverable_error() const override { return m_stream.set_recoverable_error(); }
    void set_fatal_error() const override { return m_stream.set_fatal_error(); }


};

}