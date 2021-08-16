/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libweb/html/HTMLElement.h>

namespace Web::HTML {

class HTMLTimeElement final : public HTMLElement {
public:
    using WrapperType = Bindings::HTMLTimeElementWrapper;

    HTMLTimeElement(DOM::Document&, QualifiedName);
    virtual ~HTMLTimeElement() override;
};

}
