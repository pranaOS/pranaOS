/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libcore/Forward.h>
#include <libgfx/Forward.h>
#include <libweb/html/HTMLElement.h>
#include <libweb/loader/ImageLoader.h>

namespace Web::HTML {

class HTMLObjectElement final : public HTMLElement {
public:
    using WrapperType = Bindings::HTMLObjectElementWrapper;

    HTMLObjectElement(DOM::Document&, QualifiedName);
    virtual ~HTMLObjectElement() override;

    virtual void parse_attribute(const FlyString& name, const String& value) override;

    String data() const { return attribute(HTML::AttributeNames::data); }
    String type() const { return attribute(HTML::AttributeNames::type); }

private:
    virtual RefPtr<Layout::Node> create_layout_node() override;

    ImageLoader m_image_loader;
    bool m_should_show_fallback_content { false };
};

}
