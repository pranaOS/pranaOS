/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/html/HTMLFormElement.h>
#include <libweb/html/HTMLSelectElement.h>

namespace Web::HTML {

HTMLSelectElement::HTMLSelectElement(DOM::Document& document, QualifiedName qualified_name)
    : HTMLElement(document, move(qualified_name))
{
}

HTMLSelectElement::~HTMLSelectElement()
{
}

void HTMLSelectElement::inserted()
{
    set_form(first_ancestor_of_type<HTMLFormElement>());
}

void HTMLSelectElement::removed_from(DOM::Node*)
{
    set_form(nullptr);
}

}
