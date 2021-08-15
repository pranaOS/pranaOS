/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/html/HTMLBaseElement.h>

namespace Web::HTML {

HTMLBaseElement::HTMLBaseElement(DOM::Document& document, QualifiedName qualified_name)
    : HTMLElement(document, move(qualified_name))
{
}

HTMLBaseElement::~HTMLBaseElement()
{
}

}
