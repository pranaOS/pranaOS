/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/NonnullRefPtrVector.h>
#include <base/RefCounted.h>
#include <libweb/bindings/Wrappable.h>
#include <libweb/css/CSSStyleSheet.h>
#include <libweb/Forward.h>

namespace Web::CSS {

class StyleSheetList
    : public RefCounted<StyleSheetList>
    , public Bindings::Wrappable {
public:
    using WrapperType = Bindings::StyleSheetListWrapper;

    static NonnullRefPtr<StyleSheetList> create(DOM::Document& document)
    {
        return adopt_ref(*new StyleSheetList(document));
    }

    void add_sheet(NonnullRefPtr<CSSStyleSheet>);
    const NonnullRefPtrVector<CSSStyleSheet>& sheets() const { return m_sheets; }

    RefPtr<CSSStyleSheet> item(size_t index) const
    {
        if (index >= m_sheets.size())
            return {};
        return m_sheets[index];
    }

    size_t length() const { return m_sheets.size(); }

private:
    explicit StyleSheetList(DOM::Document&);

    DOM::Document& m_document;
    NonnullRefPtrVector<CSSStyleSheet> m_sheets;
};

}

namespace Web::Bindings {

StyleSheetListWrapper* wrap(JS::GlobalObject&, CSS::StyleSheetList&);

}
