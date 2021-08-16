/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/html/HTMLLIElement.h>

namespace Web::HTML {

HTMLLIElement::HTMLLIElement(DOM::Document& document, QualifiedName qualified_name)
    : HTMLElement(document, move(qualified_name))
{
}

HTMLLIElement::~HTMLLIElement()
{
}

}
