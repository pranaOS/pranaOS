/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// incldues
#include <libweb/html/HTMLElement.h>

namespace Web::HTML {

class HTMLLabelElement final : public HTMLElement {
public:
    using WrapperType = Bindings::HTMLLabelElementWrapper;

    HTMLLabelElement(DOM::Document&, QualifiedName);
    virtual ~HTMLLabelElement() override;

    virtual RefPtr<Layout::Node> create_layout_node() override;

    String for_() const { return attribute(HTML::AttributeNames::for_); }
};

}
