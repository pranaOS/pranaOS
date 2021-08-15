/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/html/HTMLTableCaptionElement.h>

namespace Web::HTML {

HTMLTableCaptionElement::HTMLTableCaptionElement(DOM::Document& document, QualifiedName qualified_name)
    : HTMLElement(document, move(qualified_name))
{
}

HTMLTableCaptionElement::~HTMLTableCaptionElement()
{
}

}
