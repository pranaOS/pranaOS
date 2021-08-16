/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libweb/html/HTMLElement.h>

namespace Web::HTML {

class HTMLFontElement final : public HTMLElement {
public:
    using WrapperType = Bindings::HTMLFontElementWrapper;

    HTMLFontElement(DOM::Document&, QualifiedName);
    virtual ~HTMLFontElement() override;

    virtual void apply_presentational_hints(CSS::StyleProperties&) const override;
};

}
