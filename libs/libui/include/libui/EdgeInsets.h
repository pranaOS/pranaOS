/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

namespace UI {

class EdgeInsets {
public:
    EdgeInsets() = default;
    EdgeInsets(int top, int left, int bottom, int right)
        : m_top(top)
        , m_left(left)
        , m_bottom(bottom)
        , m_right(right)
    {
    }

    inline int top() const { return m_top; }
    inline int left() const { return m_left; }
    inline int bottom() const { return m_bottom; }
    inline int right() const { return m_right; }

private:
    int m_top { 0 };
    int m_bottom { 0 };
    int m_left { 0 };
    int m_right { 0 };
};

} // namespace UI