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



};

}