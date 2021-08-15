/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libweb/html/HTMLElement.h>

namespace Web::HTML {

class HTMLTitleElement final : public HTMLElement {
public:
    using WrapperType = Bindings::HTMLTitleElementWrapper;

    HTMLTitleElement(DOM::Document&, QualifiedName);
    virtual ~HTMLTitleElement() override;

private:
    virtual void children_changed() override;
};

}
