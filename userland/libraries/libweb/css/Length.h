/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/String.h>
#include <libweb/Forward.h>

namespace Web::CSS {

class Length {
public:
    enum class Type {
        Undefined,
        Percentage,
        Calculated,
        Auto,
        Cm,
        In,
        Mm,
        Q,
        Px,
        Pt,
        Pc,
        Ex,
        Em,
        Rem,
        Vh,
        Vw,
        Vmax,
        Vmin,
    };

    Length() { }
    Length(int value, Type type)
        : m_type(type)
        , m_value(value)
    {
    }
    Length(float value, Type type)
        : m_type(type)
        , m_value(value)
    {
    }

    static Length make_auto() { return Length(0, Type::Auto); }
    static Length make_px(float value) { return Length(value, Type::Px); }

    Length resolved(const Length& fallback_for_undefined, const Layout::Node& layout_node, float reference_for_percent) const
    {
        if (is_undefined())
            return fallback_for_undefined;
        if (is_calculated())
            return Length(resolve_calculated_value(layout_node, reference_for_percent), Type::Px);
        if (is_percentage())
            return make_px(raw_value() / 100.0f * reference_for_percent);
        if (is_relative())
            return make_px(to_px(layout_node));
        return *this;
    }

    Length resolved_or_auto(const Layout::Node& layout_node, float reference_for_percent) const
    {
        return resolved(make_auto(), layout_node, reference_for_percent);
    }

    Length resolved_or_zero(const Layout::Node& layout_node, float reference_for_percent) const
    {
        return resolved(make_px(0), layout_node, reference_for_percent);
    }

    bool is_undefined_or_auto() const { return m_type == Type::Undefined || m_type == Type::Auto; }
    bool is_undefined() const { return m_type == Type::Undefined; }
    bool is_percentage() const { return m_type == Type::Percentage || m_type == Type::Calculated; }
    bool is_auto() const { return m_type == Type::Auto; }
    bool is_calculated() const { return m_type == Type::Calculated; }

    bool is_absolute() const
    {
        return m_type == Type::Cm
            || m_type == Type::In
            || m_type == Type::Mm
            || m_type == Type::Px
            || m_type == Type::Pt
            || m_type == Type::Pc
            || m_type == Type::Q;
    }

    bool is_relative() const
    {
        return m_type == Type::Ex
            || m_type == Type::Em
            || m_type == Type::Rem
            || m_type == Type::Vh
            || m_type == Type::Vw
            || m_type == Type::Vmax
            || m_type == Type::Vmin;
    }

    float raw_value() const { return m_value; }
    ALWAYS_INLINE float to_px(const Layout::Node& layout_node) const
    {
        if (is_relative())
            return relative_length_to_px(layout_node);
        constexpr float inch_pixels = 96.0f;
        constexpr float centimeter_pixels = (inch_pixels / 2.54f);
        switch (m_type) {
        case Type::Auto:
            return 0;
        case Type::Cm:
            return m_value * centimeter_pixels; 
        case Type::In:
            return m_value * inch_pixels; 
        case Type::Px:
            return m_value; 
        case Type::Pt:
            return m_value * ((1.0f / 72.0f) * inch_pixels); 
        case Type::Pc:
            return m_value * ((1.0f / 6.0f) * inch_pixels); 
        case Type::Mm:
            return m_value * ((1.0f / 10.0f) * centimeter_pixels); 
        case Type::Q:
            return m_value * ((1.0f / 40.0f) * centimeter_pixels); 
        case Type::Undefined:
        case Type::Percentage:
        case Type::Calculated:
        default:
            VERIFY_NOT_REACHED();
        }
    }

    String to_string() const
    {
        if (is_auto())
            return "[auto]";
        return String::formatted("[{} {}]", m_value, unit_name());
    }

    bool operator==(const Length& other) const
    {
        return m_type == other.m_type && m_value == other.m_value;
    }

    bool operator!=(const Length& other) const
    {
        return !(*this == other);
    }

    void set_calculated_style(CalculatedStyleValue* value) { m_calculated_style = value; }

private:
    float relative_length_to_px(const Layout::Node&) const;
    float resolve_calculated_value(const Layout::Node& layout_node, float reference_for_percent) const;

    const char* unit_name() const;

    Type m_type { Type::Undefined };
    float m_value { 0 };

    CalculatedStyleValue* m_calculated_style { nullptr };
};

}
