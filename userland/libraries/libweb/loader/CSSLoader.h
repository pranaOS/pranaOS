/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/Function.h>
#include <libweb/css/CSSStyleSheet.h>
#include <libweb/loader/Resource.h>

namespace Web {

class CSSLoader : public ResourceClient {
public:
    explicit CSSLoader(DOM::Element& owner_element);

    void load_from_text(const String&);
    void load_from_url(const URL&);

    void load_next_import_if_needed();

    RefPtr<CSS::CSSStyleSheet> style_sheet() const { return m_style_sheet; };

    Function<void()> on_load;
    Function<void()> on_fail;

private:

    virtual void resource_did_load() override;
    virtual void resource_did_fail() override;

    DOM::Element& m_owner_element;

    RefPtr<CSS::CSSStyleSheet> m_style_sheet;
};

}
