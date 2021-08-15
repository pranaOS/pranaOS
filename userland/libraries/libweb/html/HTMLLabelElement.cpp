/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/dom/Document.h>
#include <libweb/html/HTMLLabelElement.h>
#include <libweb/layout/Label.h>

namespace Web::HTML {

HTMLLabelElement::HTMLLabelElement(DOM::Document& document, QualifiedName qualified_name)
    : HTMLElement(document, move(qualified_name))
{
}

HTMLLabelElement::~HTMLLabelElement()
{
}

RefPtr<Layout::Node> HTMLLabelElement::create_layout_node()
{
    auto style = document().style_resolver().resolve_style(*this);
    if (style->display() == CSS::Display::None)
        return nullptr;

    auto layout_node = adopt_ref(*new Layout::Label(document(), this, move(style)));
    layout_node->set_inline(true);
    return layout_node;
}

}
