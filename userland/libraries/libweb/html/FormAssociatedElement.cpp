/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/html/FormAssociatedElement.h>
#include <libweb/html/HTMLFormElement.h>

namespace Web::HTML {

void FormAssociatedElement::set_form(HTMLFormElement* form)
{
    if (m_form)
        m_form->remove_associated_element({}, form_associated_element_to_html_element());
    m_form = form;
    if (m_form)
        m_form->add_associated_element({}, form_associated_element_to_html_element());
}

}
