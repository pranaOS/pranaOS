/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libweb/html/HTMLElement.h>

namespace Web::HTML {

class HTMLDetailsElement final : public HTMLElement {
public:
    using WrapperType = Bindings::HTMLDetailsElementWrapper;

    HTMLDetailsElement(DOM::Document&, QualifiedName);
    virtual ~HTMLDetailsElement() override;
};

}
