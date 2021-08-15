/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <base/URL.h>
#include <libweb/css/CSSImportRule.h>
#include <libweb/css/CSSStyleSheet.h>

namespace Web::CSS {

CSSImportRule::CSSImportRule(URL url)
    : m_url(move(url))
{
}

CSSImportRule::~CSSImportRule()
{
}

}
