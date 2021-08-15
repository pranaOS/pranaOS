/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/css/CSSStyleDeclaration.h>
#include <libweb/dom/Element.h>

namespace Web::CSS {

CSSStyleDeclaration::CSSStyleDeclaration(Vector<StyleProperty>&& properties, HashMap<String, StyleProperty>&& custom_properties)
    : m_properties(move(properties))
    , m_custom_properties(move(custom_properties))
{
}

CSSStyleDeclaration::~CSSStyleDeclaration()
{
}

String CSSStyleDeclaration::item(size_t index) const
{
    if (index >= m_properties.size())
        return {};
    return CSS::string_from_property_id(m_properties[index].property_id);
}

ElementInlineCSSStyleDeclaration::ElementInlineCSSStyleDeclaration(DOM::Element& element)
    : CSSStyleDeclaration({}, {})
    , m_element(element.make_weak_ptr<DOM::Element>())
{
}

ElementInlineCSSStyleDeclaration::~ElementInlineCSSStyleDeclaration()
{
}

}
