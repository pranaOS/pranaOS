/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/html/HTMLTimeElement.h>

namespace Web::HTML {

HTMLTimeElement::HTMLTimeElement(DOM::Document& document, QualifiedName qualified_name)
    : HTMLElement(document, move(qualified_name))
{
}

HTMLTimeElement::~HTMLTimeElement()
{
}

}
