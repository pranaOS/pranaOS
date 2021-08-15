/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libweb/html/FormAssociatedElement.h>
#include <libweb/html/HTMLElement.h>

namespace Web::HTML {

class HTMLSelectElement final
    : public HTMLElement
    , public FormAssociatedElement {
public:
    using WrapperType = Bindings::HTMLSelectElementWrapper;

    HTMLSelectElement(DOM::Document&, QualifiedName);
    virtual ~HTMLSelectElement() override;

private:

    virtual void inserted() override;
    virtual void removed_from(DOM::Node*) override;

    virtual HTMLElement& form_associated_element_to_html_element() override { return *this; }
};

}
