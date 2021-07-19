/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Span.h>
#include <base/StdLibExtraDetails.h>

namespace Base 
{

class BinaryBufferWriter
{
public:
    BinaryBufferWriter(Bytes target)
        : m_target(target)
    {
    }

    template<typename T>
    requires(Base::Detail::IsTriviallyConstructible<T>) T& append_structure()
    {
        VERIFY((reinterpret_cast<FlatPtr>(m_target.data()) + m_offset) % alignof(T) == 0);
        VERIFY(m_offset + sizeof(T) <= m_target.size());
        T* allocated = new (m_target.data() + m_offset) T;
        m_offset += sizeof(T);
        return *allocated;
    }



};

}