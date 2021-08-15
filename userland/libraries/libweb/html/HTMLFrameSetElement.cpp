/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/html/HTMLFrameSetElement.h>

namespace Web::HTML {

HTMLFrameSetElement::HTMLFrameSetElement(DOM::Document& document, QualifiedName qualified_name)
    : HTMLElement(document, move(qualified_name))
{
}

HTMLFrameSetElement::~HTMLFrameSetElement()
{
}

}
