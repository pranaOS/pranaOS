/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/dom/DOMImplementation.h>
#include <libweb/dom/Document.h>
#include <libweb/dom/DocumentType.h>
#include <libweb/dom/ElementFactory.h>
#include <libweb/dom/Text.h>
#include <libweb/Namespace.h>
#include <libweb/Origin.h>

namespace Web::DOM {

DOMImplementation::DOMImplementation(Document& document)
    : m_document(document)
{
}

NonnullRefPtr<Document> DOMImplementation::create_document(const String& namespace_, const String& qualified_name) const
{
    auto xml_document = Document::create();

    xml_document->set_ready_for_post_load_tasks(true);

    RefPtr<Element> element;

    if (!qualified_name.is_empty())
        element = xml_document->create_element_ns(namespace_, qualified_name /* FIXME: and an empty dictionary */);


    if (element)
        xml_document->append_child(element.release_nonnull());

    xml_document->set_origin(m_document.origin());

    if (namespace_ == Namespace::HTML)
        m_document.set_content_type("application/xhtml+xml");
    else if (namespace_ == Namespace::SVG)
        m_document.set_content_type("image/svg+xml");
    else
        m_document.set_content_type("application/xml");

    return xml_document;
}

NonnullRefPtr<Document> DOMImplementation::create_html_document(const String& title) const
{
    auto html_document = Document::create();

    html_document->set_content_type("text/html");
    html_document->set_ready_for_post_load_tasks(true);

    auto doctype = adopt_ref(*new DocumentType(html_document));
    doctype->set_name("html");
    html_document->append_child(doctype);

    auto html_element = create_element(html_document, HTML::TagNames::html, Namespace::HTML);
    html_document->append_child(html_element);

    auto head_element = create_element(html_document, HTML::TagNames::head, Namespace::HTML);
    html_element->append_child(head_element);

    if (!title.is_null()) {
        auto title_element = create_element(html_document, HTML::TagNames::title, Namespace::HTML);
        head_element->append_child(title_element);

        auto text_node = adopt_ref(*new Text(html_document, title));
        title_element->append_child(text_node);
    }

    auto body_element = create_element(html_document, HTML::TagNames::body, Namespace::HTML);
    html_element->append_child(body_element);

    html_document->set_origin(m_document.origin());

    return html_document;
}

NonnullRefPtr<DocumentType> DOMImplementation::create_document_type(const String& qualified_name, const String& public_id, const String& system_id) const
{
    auto document_type = DocumentType::create(m_document);
    document_type->set_name(qualified_name);
    document_type->set_public_id(public_id);
    document_type->set_system_id(system_id);
    return document_type;
}

}
