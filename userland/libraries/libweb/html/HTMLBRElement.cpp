/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// include
#include <libweb/html/HTMLBRElement.h>
#include <libweb/layout/BreakNode.h>

namespace Web::HTML {

HTMLBRElement::HTMLBRElement(DOM::Document& document, QualifiedName qualified_name)
    : HTMLElement(document, move(qualified_name))
{
}

HTMLBRElement::~HTMLBRElement()
{
}

RefPtr<Layout::Node> HTMLBRElement::create_layout_node()
{
    return adopt_ref(*new Layout::BreakNode(document(), *this));
}

}
