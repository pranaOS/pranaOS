/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/html/HTMLFrameElement.h>

namespace Web::HTML {

HTMLFrameElement::HTMLFrameElement(DOM::Document& document, QualifiedName qualified_name)
    : HTMLElement(document, move(qualified_name))
{
}

HTMLFrameElement::~HTMLFrameElement()
{
}

}
