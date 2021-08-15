/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/css/CSSStyleRule.h>

namespace Web::CSS {

CSSStyleRule::CSSStyleRule(NonnullRefPtrVector<Selector>&& selectors, NonnullRefPtr<CSSStyleDeclaration>&& declaration)
    : m_selectors(move(selectors))
    , m_declaration(move(declaration))
{
}

CSSStyleRule::~CSSStyleRule()
{
}

}
