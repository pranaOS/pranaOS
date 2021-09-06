/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <cstddef>
#include <sys/types.h>

namespace LG {

class CornerMask {
public:
    static constexpr bool Masked = true;
    static constexpr bool NonMasked = false;
    static constexpr size_t SystemRadius = 4;

    CornerMask(size_t radius, bool top_rounded = Masked, bool bottom_rounded = Masked)
        : m_radius(radius)
        , m_top_rounded(top_rounded)
        , m_bottom_rounded(bottom_rounded)
    {
    }

    size_t radius() const { return m_radius; }
    bool top_rounded() const { return m_top_rounded; }
    bool bottom_rounded() const { return m_bottom_rounded; }

private:
    size_t m_radius { SystemRadius };
    bool m_top_rounded { Masked };
    bool m_bottom_rounded { Masked };
};

} // namespace LG