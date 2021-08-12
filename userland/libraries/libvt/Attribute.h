/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Noncopyable.h>
#include <base/String.h>
#include <base/Vector.h>
#include <libvt/Color.h>
#include <libvt/XtermColors.h>

namespace VT {

struct Attribute {
    static constexpr Color default_foreground_color = Color::named(Color::ANSIColor::DefaultForeground);
    static constexpr Color default_background_color = Color::named(Color::ANSIColor::DefaultBackground);

    void reset()
    {
        foreground_color = default_foreground_color;
        background_color = default_background_color;
        flags = Flags::NoAttributes;
#ifndef KERNEL
        href = {};
        href_id = {};
#endif
    }

    Color foreground_color { default_foreground_color };
    Color background_color { default_background_color };

    constexpr Color effective_background_color() const { return flags & Negative ? foreground_color : background_color; }
    constexpr Color effective_foreground_color() const { return flags & Negative ? background_color : foreground_color; }

#ifndef KERNEL
    String href;
    String href_id;
#endif

    enum Flags : u8 {
        NoAttributes = 0x00,
        Bold = 0x01,
        Italic = 0x02,
        Underline = 0x04,
        Negative = 0x08,
        Blink = 0x10,
        Touched = 0x20,
    };

    constexpr bool is_untouched() const { return !(flags & Touched); }

    u8 flags { Flags::NoAttributes };

    constexpr bool operator==(const Attribute& other) const
    {
        return foreground_color == other.foreground_color && background_color == other.background_color && flags == other.flags;
    }
    constexpr bool operator!=(const Attribute& other) const
    {
        return !(*this == other);
    }
};

}
