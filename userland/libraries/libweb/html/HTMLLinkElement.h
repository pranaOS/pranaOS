/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libweb/html/HTMLElement.h>
#include <libweb/loader/CSSLoader.h>

namespace Web::HTML {

class HTMLLinkElement final : public HTMLElement {
public:
    using WrapperType = Bindings::HTMLLinkElementWrapper;

    HTMLLinkElement(DOM::Document&, QualifiedName);
    virtual ~HTMLLinkElement() override;

    virtual void inserted() override;

    String rel() const { return attribute(HTML::AttributeNames::rel); }
    String type() const { return attribute(HTML::AttributeNames::type); }
    String href() const { return attribute(HTML::AttributeNames::href); }

private:
    void parse_attribute(const FlyString&, const String&) override;

    struct Relationship {
        enum {
            Alternate = 1 << 0,
            Stylesheet = 1 << 1,
        };
    };

    CSSLoader m_css_loader;
    unsigned m_relationship { 0 };
};

}
