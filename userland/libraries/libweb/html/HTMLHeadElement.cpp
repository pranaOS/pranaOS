/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/html/HTMLHeadElement.h>

namespace Web::HTML {

HTMLHeadElement::HTMLHeadElement(DOM::Document& document, QualifiedName qualified_name)
    : HTMLElement(document, move(qualified_name))
{
}

HTMLHeadElement::~HTMLHeadElement()
{
}

}
