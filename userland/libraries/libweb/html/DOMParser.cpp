/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/html/DOMParser.h>
#include <libweb/html/parser/HTMLDocumentParser.h>

namespace Web::HTML {

DOMParser::DOMParser()
{
}

DOMParser::~DOMParser()
{
}

NonnullRefPtr<DOM::Document> DOMParser::parse_from_string(String const& string, String const& type)
{
    auto document = DOM::Document::create();
    document->set_content_type(type);

    if (type == "text/html") {
        HTMLDocumentParser parser(document, string, "UTF-8");
        parser.run("about:blank");
    } else {
        dbgln("DOMParser::parse_from_string: Unimplemented parser for type: {}", type);
        TODO();
    }

    return document;
}

}
