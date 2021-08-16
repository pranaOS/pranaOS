/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/String.h>
#include <libweb/dom/Node.h>
#include <libweb/dom/NonDocumentTypeChildNode.h>

namespace Web::DOM {

class CharacterData
    : public Node
    , public NonDocumentTypeChildNode<CharacterData> {
public:
    using WrapperType = Bindings::CharacterDataWrapper;

    virtual ~CharacterData() override;

    const String& data() const { return m_data; }
    void set_data(String);

    unsigned length() const { return m_data.length(); }

    virtual String text_content() const override { return m_data; }

protected:
    explicit CharacterData(Document&, NodeType, const String&);

private:
    String m_data;
};

}
