/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// inlcudes
#include <libweb/html/HTMLElement.h>

namespace Web::HTML {

class HTMLMediaElement : public HTMLElement {
public:
    using WrapperType = Bindings::HTMLMediaElementWrapper;

    HTMLMediaElement(DOM::Document&, QualifiedName);
    virtual ~HTMLMediaElement() override;
};

}
