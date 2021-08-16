/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/html/HTMLAudioElement.h>

namespace Web::HTML {

HTMLAudioElement::HTMLAudioElement(DOM::Document& document, QualifiedName qualified_name)
    : HTMLMediaElement(document, move(qualified_name))
{
}

HTMLAudioElement::~HTMLAudioElement()
{
}

}
