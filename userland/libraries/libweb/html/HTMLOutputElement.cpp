/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/html/HTMLOutputElement.h>

namespace Web::HTML {

HTMLOutputElement::HTMLOutputElement(DOM::Document& document, QualifiedName qualified_name)
    : HTMLElement(document, move(qualified_name))
{
}

HTMLOutputElement::~HTMLOutputElement()
{
}

}
