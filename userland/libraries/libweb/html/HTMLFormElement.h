/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libweb/html/HTMLElement.h>
#include <libweb/html/HTMLInputElement.h>

namespace Web::HTML {

class HTMLFormElement final : public HTMLElement {
public:
    using WrapperType = Bindings::HTMLFormElementWrapper;

    HTMLFormElement(DOM::Document&, QualifiedName);
    virtual ~HTMLFormElement() override;

    String action() const { return attribute(HTML::AttributeNames::action); }
    String method() const { return attribute(HTML::AttributeNames::method); }

    void submit_form(RefPtr<HTMLElement> submitter, bool from_submit_binding = false);

    void submit();

    void add_associated_element(Badge<FormAssociatedElement>, HTMLElement&);
    void remove_associated_element(Badge<FormAssociatedElement>, HTMLElement&);

private:
    bool m_firing_submission_events { false };

    Vector<WeakPtr<HTMLElement>> m_associated_elements;
};

}
