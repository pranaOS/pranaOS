/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <base/Debug.h>
#include <base/URL.h>
#include <libweb/css/Parser/Parser.h>
#include <libweb/dom/Document.h>
#include <libweb/dom/Element.h>
#include <libweb/loader/CSSLoader.h>
#include <libweb/loader/ResourceLoader.h>

namespace Web {

CSSLoader::CSSLoader(DOM::Element& owner_element)
    : m_owner_element(owner_element)
{
}

void CSSLoader::load_from_text(const String& text)
{
    m_style_sheet = parse_css(CSS::ParsingContext(m_owner_element.document()), text);
    if (!m_style_sheet) {
        m_style_sheet = CSS::CSSStyleSheet::create({});
        m_style_sheet->set_owner_node(&m_owner_element);
    }

    load_next_import_if_needed();
}

void CSSLoader::load_from_url(const URL& url)
{
    m_style_sheet = CSS::CSSStyleSheet::create({});
    m_style_sheet->set_owner_node(&m_owner_element);

    auto request = LoadRequest::create_for_url_on_page(url, m_owner_element.document().page());
    set_resource(ResourceLoader::the().load_resource(Resource::Type::Generic, request));
}

void CSSLoader::resource_did_load()
{
    VERIFY(resource());

    if (!resource()->has_encoded_data()) {
        dbgln_if(CSS_LOADER_DEBUG, "CSSLoader: Resource did load, no encoded data. URL: {}", resource()->url());
    } else {
        dbgln_if(CSS_LOADER_DEBUG, "CSSLoader: Resource did load, has encoded data. URL: {}", resource()->url());
    }

    auto sheet = parse_css(CSS::ParsingContext(m_owner_element.document()), resource()->encoded_data());
    if (!sheet) {
        dbgln_if(CSS_LOADER_DEBUG, "CSSLoader: Failed to parse stylesheet: {}", resource()->url());
        return;
    }

    bool was_imported = m_style_sheet->for_first_not_loaded_import_rule([&](auto& rule) {
        rule.set_style_sheet(sheet);
    });

    if (!was_imported) {
        m_style_sheet->rules() = sheet->rules();
    }

    if (on_load)
        on_load();

    load_next_import_if_needed();
}

void CSSLoader::resource_did_fail()
{
    dbgln_if(CSS_LOADER_DEBUG, "CSSLoader: Resource did fail. URL: {}", resource()->url());

    load_next_import_if_needed();
}

void CSSLoader::load_next_import_if_needed()
{
    m_style_sheet->for_first_not_loaded_import_rule([&](auto& rule) {
        dbgln_if(CSS_LOADER_DEBUG, "CSSLoader: Loading @import {}", rule.url());

        LoadRequest request;
        request.set_url(rule.url());
        set_resource(ResourceLoader::the().load_resource(Resource::Type::Generic, request));
    });
}

}
