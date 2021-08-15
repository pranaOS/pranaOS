/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/html/HTMLTrackElement.h>

namespace Web::HTML {

HTMLTrackElement::HTMLTrackElement(DOM::Document& document, QualifiedName qualified_name)
    : HTMLElement(document, move(qualified_name))
{
}

HTMLTrackElement::~HTMLTrackElement()
{
}

}
