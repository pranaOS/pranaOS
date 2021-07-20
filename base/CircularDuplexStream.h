/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/CircularQueue.h>
#include <base/Stream.h>

namespace Base {

template<size_t Capacity>
class CircularDuplexStream : public AK::DuplexStream {
public:
    size_t write(ReadonlyBytes bytes) override
    {
        const auto nwritten = min(bytes.size(), Capacity - m_queue.size());

        for (size_t idx = 0; idx < nwritten; ++idx)
            m_queue.enqueue(bytes[idx]);

        m_total_written += nwritten;
        return nwritten;
    }

    bool write_or_error(ReadonlyBytes bytes) override
    {
        if (Capacity - m_queue.size() < bytes.size()) {
            set_recoverable_error();
            return false
        }

        const auto nwritten = write(bytes);
        VERIFY(nwritten == bytes.size());
        return true;
    }
};
}