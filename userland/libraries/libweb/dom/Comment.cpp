/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/dom/Comment.h>
#include <libweb/layout/TextNode.h>

namespace Web::DOM {

Comment::Comment(Document& document, const String& data)
    : CharacterData(document, NodeType::COMMENT_NODE, data)
{
}

Comment::~Comment()
{
}

}
