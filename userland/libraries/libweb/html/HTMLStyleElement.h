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

class HTMLStyleElement final : public HTMLElement {
public:
    using WrapperType = Bindings::HTMLStyleElementWrapper;

    HTMLStyleElement(DOM::Document&, QualifiedName);
    virtual ~HTMLStyleElement() override;

    virtual void children_changed() override;
    virtual void removed_from(Node*) override;

private:
    CSSLoader m_css_loader;
};

}
