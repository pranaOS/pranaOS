/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/html/HTMLButtonElement.h>

namespace Web::HTML {

HTMLButtonElement::HTMLButtonElement(DOM::Document& document, QualifiedName qualified_name)
    : HTMLElement(document, move(qualified_name))
{
}

HTMLButtonElement::~HTMLButtonElement()
{
}

}
