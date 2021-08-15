/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/html/HTMLTableRowElement.h>

namespace Web::HTML {

HTMLTableRowElement::HTMLTableRowElement(DOM::Document& document, QualifiedName qualified_name)
    : HTMLElement(document, move(qualified_name))
{
}

HTMLTableRowElement::~HTMLTableRowElement()
{
}

}
