/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/css/StyleSheetList.h>

namespace Web::CSS {

void StyleSheetList::add_sheet(NonnullRefPtr<CSSStyleSheet> sheet)
{
    m_sheets.append(move(sheet));
}

StyleSheetList::StyleSheetList(DOM::Document& document)
    : m_document(document)
{
}

}
