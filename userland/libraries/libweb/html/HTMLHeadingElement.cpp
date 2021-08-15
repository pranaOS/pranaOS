/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/html/HTMLHeadingElement.h>

namespace Web::HTML {

HTMLHeadingElement::HTMLHeadingElement(DOM::Document& document, QualifiedName qualified_name)
    : HTMLElement(document, move(qualified_name))
{
}

HTMLHeadingElement::~HTMLHeadingElement()
{
}

}
