/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/FlyString.h>
#include <libweb/dom/Element.h>
#include <libweb/dom/NonElementParentNode.h>
#include <libweb/dom/ParentNode.h>

namespace Web::DOM {

class DocumentFragment
    : public ParentNode
    , public NonElementParentNode<DocumentFragment> {
public:
    using WrapperType = Bindings::DocumentFragmentWrapper;

    explicit DocumentFragment(Document& document);
    virtual ~DocumentFragment() override;

    virtual FlyString node_name() const override { return "#document-fragment"; }

    RefPtr<Element> host() { return m_host; }
    const RefPtr<Element> host() const { return m_host; }

    void set_host(Element& host) { m_host = host; }

private:
    RefPtr<Element> m_host;
};

}
