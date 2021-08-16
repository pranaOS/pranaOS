/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/HashMap.h>
#include <libweb/css/StyleResolver.h>
#include <libweb/dom/Document.h>
#include <libweb/dom/Element.h>

namespace Web::CSS {

class StyleInvalidator {
public:
    explicit StyleInvalidator(DOM::Document&);
    ~StyleInvalidator();

private:
    DOM::Document& m_document;
    HashMap<DOM::Element*, Vector<MatchingRule>> m_elements_and_matching_rules_before;
};

}
