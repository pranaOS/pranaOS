/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/html/HTMLQuoteElement.h>

namespace Web::HTML {

HTMLQuoteElement::HTMLQuoteElement(DOM::Document& document, QualifiedName qualified_name)
    : HTMLElement(document, move(qualified_name))
{
}

HTMLQuoteElement::~HTMLQuoteElement()
{
}

}
