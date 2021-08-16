/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libweb/html/HTMLElement.h>

namespace Web::HTML {

class HTMLBRElement final : public HTMLElement {
public:
    using WrapperType = Bindings::HTMLBRElementWrapper;

    HTMLBRElement(DOM::Document&, QualifiedName);
    virtual ~HTMLBRElement() override;

    virtual RefPtr<Layout::Node> create_layout_node() override;
};

}
