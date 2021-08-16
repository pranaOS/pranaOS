/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/css/CSSStyleSheet.h>

namespace Web::CSS {

CSSStyleSheet::CSSStyleSheet(NonnullRefPtrVector<CSSRule> rules)
    : m_rules(move(rules))
{
}

CSSStyleSheet::~CSSStyleSheet()
{
}

}
