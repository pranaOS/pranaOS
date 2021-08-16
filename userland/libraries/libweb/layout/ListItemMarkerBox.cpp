/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <base/StringBuilder.h>
#include <libgfx/Painter.h>
#include <libweb/layout/ListItemMarkerBox.h>

namespace Web::Layout {

ListItemMarkerBox::ListItemMarkerBox(DOM::Document& document, CSS::ListStyleType style_type, size_t index)
    : Box(document, nullptr, CSS::StyleProperties::create())
    , m_list_style_type(style_type)
    , m_index(index)
{
    switch (m_list_style_type) {
    case CSS::ListStyleType::Square:
    case CSS::ListStyleType::Circle:
    case CSS::ListStyleType::Disc:
        break;
    case CSS::ListStyleType::Decimal:
        m_text = String::formatted("{}.", m_index);
        break;
    case CSS::ListStyleType::DecimalLeadingZero:
        m_text = m_index < 10 ? String::formatted("0{}.", m_index) : String::formatted("{}.", m_index);
        break;
    case CSS::ListStyleType::LowerAlpha:
    case CSS::ListStyleType::LowerLatin:
        m_text = String::bijective_base_from(m_index - 1).to_lowercase();
        break;
    case CSS::ListStyleType::UpperAlpha:
    case CSS::ListStyleType::UpperLatin:
        m_text = String::bijective_base_from(m_index - 1);
        break;
    case CSS::ListStyleType::LowerRoman:
        m_text = String::roman_number_from(m_index).to_lowercase();
        break;
    case CSS::ListStyleType::UpperRoman:
        m_text = String::roman_number_from(m_index);
        break;
    case CSS::ListStyleType::None:
        break;

    default:
        VERIFY_NOT_REACHED();
    }

    if (m_text.is_null()) {
        set_width(4);
        return;
    }

    auto text_width = font().width(m_text);
    set_width(text_width);
}

ListItemMarkerBox::~ListItemMarkerBox()
{
}

void ListItemMarkerBox::paint(PaintContext& context, PaintPhase phase)
{
    if (phase != PaintPhase::Foreground)
        return;

    auto color = parent()->computed_values().color();

    auto enclosing = enclosing_int_rect(absolute_rect());
    int marker_width = (int)enclosing.height() / 2;
    Gfx::IntRect marker_rect { 0, 0, marker_width, marker_width };
    marker_rect.center_within(enclosing);

    switch (m_list_style_type) {
    case CSS::ListStyleType::Square:
        context.painter().fill_rect(marker_rect, color);
        break;
    case CSS::ListStyleType::Circle:
        marker_rect.set_height(marker_rect.height() / 1.41);
        marker_rect.set_width(marker_rect.width() / 1.41);
        marker_rect.center_within(enclosing);
        context.painter().draw_ellipse_intersecting(marker_rect, color);
        break;
    case CSS::ListStyleType::Disc:
        context.painter().fill_ellipse(marker_rect, color);
        break;
    case CSS::ListStyleType::Decimal:
    case CSS::ListStyleType::DecimalLeadingZero:
    case CSS::ListStyleType::LowerAlpha:
    case CSS::ListStyleType::LowerLatin:
    case CSS::ListStyleType::LowerRoman:
    case CSS::ListStyleType::UpperAlpha:
    case CSS::ListStyleType::UpperLatin:
    case CSS::ListStyleType::UpperRoman:
        if (m_text.is_null())
            break;
        context.painter().draw_text(enclosing, m_text, Gfx::TextAlignment::Center);
        break;
    case CSS::ListStyleType::None:
        return;

    default:
        VERIFY_NOT_REACHED();
    }
}

}
