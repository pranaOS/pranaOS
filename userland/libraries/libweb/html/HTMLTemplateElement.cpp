/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/dom/Document.h>
#include <libweb/html/HTMLTemplateElement.h>

namespace Web::HTML {

HTMLTemplateElement::HTMLTemplateElement(DOM::Document& document, QualifiedName qualified_name)
    : HTMLElement(document, move(qualified_name))
{
    m_content = adopt_ref(*new DOM::DocumentFragment(appropriate_template_contents_owner_document(document)));
    m_content->set_host(*this);
}

HTMLTemplateElement::~HTMLTemplateElement()
{
}

DOM::Document& HTMLTemplateElement::appropriate_template_contents_owner_document(DOM::Document& document)
{
    if (!document.created_for_appropriate_template_contents()) {
        if (!document.associated_inert_template_document()) {
            auto new_document = DOM::Document::create();
            new_document->set_created_for_appropriate_template_contents(true);

            document.set_associated_inert_template_document(new_document);
        }

        return *document.associated_inert_template_document();
    }

    return document;
}

void HTMLTemplateElement::adopted_from(DOM::Document&)
{
    appropriate_template_contents_owner_document(document()).adopt_node(content());
}

void HTMLTemplateElement::cloned(Node& copy, bool clone_children)
{
    if (!clone_children)
        return;

    auto& template_clone = verify_cast<HTMLTemplateElement>(copy);

    content()->for_each_child([&](auto& child) {
        auto cloned_child = child.clone_node(&template_clone.content()->document(), true);

        template_clone.content()->append_child(cloned_child);
    });
}

}
