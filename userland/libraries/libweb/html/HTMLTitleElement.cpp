/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/dom/Document.h>
#include <libweb/html/HTMLTitleElement.h>
#include <libweb/page/Page.h>

namespace Web::HTML {

HTMLTitleElement::HTMLTitleElement(DOM::Document& document, QualifiedName qualified_name)
    : HTMLElement(document, move(qualified_name))
{
}

HTMLTitleElement::~HTMLTitleElement()
{
}

void HTMLTitleElement::children_changed()
{
    HTMLElement::children_changed();
    if (auto* page = document().page()) {
        if (document().browsing_context() == &page->top_level_browsing_context())
            page->client().page_did_change_title(document().title());
    }
}

}
