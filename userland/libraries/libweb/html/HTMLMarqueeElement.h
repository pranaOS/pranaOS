/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libweb/html/HTMLElement.h>

namespace Web::HTML {

class HTMLMarqueeElement final : public HTMLElement {
public:
    using WrapperType = Bindings::HTMLMarqueeElementWrapper;

    HTMLMarqueeElement(DOM::Document&, QualifiedName);
    virtual ~HTMLMarqueeElement() override;

private:
    virtual void apply_presentational_hints(CSS::StyleProperties&) const override;
};

}
