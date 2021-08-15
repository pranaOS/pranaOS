/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/WeakPtr.h>
#include <libweb/Forward.h>

namespace Web::HTML {

class FormAssociatedElement {
public:
    HTMLFormElement* form() { return m_form; }
    HTMLFormElement const* form() const { return m_form; }

    void set_form(HTMLFormElement*);

protected:
    FormAssociatedElement() = default;
    virtual ~FormAssociatedElement() = default;

    virtual HTMLElement& form_associated_element_to_html_element() = 0;

private:
    WeakPtr<HTMLFormElement> m_form;
};

}
