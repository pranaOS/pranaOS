/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/html/HTMLMapElement.h>

namespace Web::HTML {

HTMLMapElement::HTMLMapElement(DOM::Document& document, QualifiedName qualified_name)
    : HTMLElement(document, move(qualified_name))
{
}

HTMLMapElement::~HTMLMapElement()
{
}

}
