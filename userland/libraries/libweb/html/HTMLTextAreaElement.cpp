/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/html/HTMLTextAreaElement.h>

namespace Web::HTML {

HTMLTextAreaElement::HTMLTextAreaElement(DOM::Document& document, QualifiedName qualified_name)
    : HTMLElement(document, move(qualified_name))
{
}

HTMLTextAreaElement::~HTMLTextAreaElement()
{
}

}
