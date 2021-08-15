/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <base/Utf8View.h>
#include <libweb/dom/Node.h>
#include <libweb/dom/Position.h>
#include <libweb/dom/Text.h>

namespace Web::DOM {

Position::Position(Node& node, unsigned offset)
    : m_node(node)
    , m_offset(offset)
{
}

Position::~Position()
{
}

String Position::to_string() const
{
    if (!node())
        return String::formatted("DOM::Position(nullptr, {})", offset());
    return String::formatted("DOM::Position({} ({})), {})", node()->node_name(), node(), offset());
}

bool Position::increment_offset()
{
    if (!is<DOM::Text>(*m_node))
        return false;

    auto& node = verify_cast<DOM::Text>(*m_node);
    auto text = Utf8View(node.data());

    for (auto iterator = text.begin(); !iterator.done(); ++iterator) {
        if (text.byte_offset_of(iterator) >= m_offset) {
            // NOTE: If the current offset is inside a multi-byte code point, it will be moved to the start of the next code point.
            m_offset = text.byte_offset_of(++iterator);
            return true;
        }
    }
    // NOTE: Already at end of current node.
    return false;
}

bool Position::decrement_offset()
{
    if (m_offset == 0 || !is<DOM::Text>(*m_node))
        return false;

    auto& node = verify_cast<DOM::Text>(*m_node);
    auto text = Utf8View(node.data());

    size_t last_smaller_offset = 0;

    for (auto iterator = text.begin(); !iterator.done(); ++iterator) {
        auto byte_offset = text.byte_offset_of(iterator);
        if (byte_offset >= m_offset) {
            break;
        }
        last_smaller_offset = text.byte_offset_of(iterator);
    }

    // NOTE: If the current offset is inside a multi-byte code point, it will be moved to the start of that code point.
    m_offset = last_smaller_offset;
    return true;
}

bool Position::offset_is_at_end_of_node() const
{
    if (!is<DOM::Text>(*m_node))
        return false;

    auto& node = verify_cast<DOM::Text>(*m_node);
    auto text = node.data();
    return m_offset == text.length();
}

}
