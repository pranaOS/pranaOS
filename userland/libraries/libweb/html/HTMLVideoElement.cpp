/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/html/HTMLVideoElement.h>

namespace Web::HTML {

HTMLVideoElement::HTMLVideoElement(DOM::Document& document, QualifiedName qualified_name)
    : HTMLMediaElement(document, move(qualified_name))
{
}

HTMLVideoElement::~HTMLVideoElement()
{
}

}
