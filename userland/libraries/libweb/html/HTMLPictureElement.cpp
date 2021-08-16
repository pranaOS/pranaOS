/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/html/HTMLPictureElement.h>

namespace Web::HTML {

HTMLPictureElement::HTMLPictureElement(DOM::Document& document, QualifiedName qualified_name)
    : HTMLElement(document, move(qualified_name))
{
}

HTMLPictureElement::~HTMLPictureElement()
{
}

}
