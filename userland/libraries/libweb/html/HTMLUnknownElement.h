/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libweb/html/HTMLElement.h>

namespace Web::HTML {

class HTMLUnknownElement final : public HTMLElement {
public:
    using WrapperType = Bindings::HTMLUnknownElementWrapper;

    HTMLUnknownElement(DOM::Document&, QualifiedName);
    virtual ~HTMLUnknownElement() override;
};

}
