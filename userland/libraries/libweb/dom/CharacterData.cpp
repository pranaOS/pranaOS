/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/dom/CharacterData.h>
#include <libweb/dom/Document.h>

namespace Web::DOM {

CharacterData::CharacterData(Document& document, NodeType type, const String& data)
    : Node(document, type)
    , m_data(data)
{
}

CharacterData::~CharacterData()
{
}

void CharacterData::set_data(String data)
{
    if (m_data == data)
        return;
    m_data = move(data);

    document().schedule_forced_layout();
}

}
