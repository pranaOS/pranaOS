/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/html/HTMLEmbedElement.h>

namespace Web::HTML {

HTMLEmbedElement::HTMLEmbedElement(DOM::Document& document, QualifiedName qualified_name)
    : HTMLElement(document, move(qualified_name))
{
}

HTMLEmbedElement::~HTMLEmbedElement()
{
}

}
