/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

/* includes */
#include <cstdint>
#include <libg/Color.h>

struct StatusBarStyle {
public:
    enum Mode : uint32_t {
        LightText = (1 << 0),
        HideText = (1 << 1),
    };

    StatusBarStyle()
        : m_flags(0)
        , m_color(LG::Color::LightSystemBackground)
    {
    }

    StatusBarStyle(uint32_t attr)
        : m_flags(attr)
    {
    }

    StatusBarStyle(Mode attr)
        : m_flags(uint32_t(attr))
    {
    }

    StatusBarStyle(uint32_t attr, const LG::Color& clr)
        : m_flags(attr)
        , m_color(clr)
    {
    }

    StatusBarStyle(Mode attr, const LG::Color& clr)
        : m_flags(uint32_t(attr))
        , m_color(clr)
    {

    }

    explicit StatusBarStyle(Mode attr, const LG::Color& clr)
        : m_flags(uint32_t(attr))
        , m_color(clr)
    {
    }

};