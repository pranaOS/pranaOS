/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/dom/Text.h>
#include <libweb/layout/TextNode.h>

namespace Web::DOM {

Text::Text(Document& document, const String& data)
    : CharacterData(document, NodeType::TEXT_NODE, data)
{
}

Text::~Text()
{
}

RefPtr<Layout::Node> Text::create_layout_node()
{
    return adopt_ref(*new Layout::TextNode(document(), *this));
}

}
