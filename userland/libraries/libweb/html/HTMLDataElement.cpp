/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/html/HTMLDataElement.h>

namespace Web::HTML {

HTMLDataElement::HTMLDataElement(DOM::Document& document, QualifiedName qualified_name)
    : HTMLElement(document, move(qualified_name))
{
}

HTMLDataElement::~HTMLDataElement()
{
}

}
