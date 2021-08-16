/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libweb/html/HTMLElement.h>

namespace Web::HTML {

class HTMLParamElement final : public HTMLElement {
public:
    using WrapperType = Bindings::HTMLParamElementWrapper;

    HTMLParamElement(DOM::Document&, QualifiedName);
    virtual ~HTMLParamElement() override;
};

}
