/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libweb/html/HTMLElement.h>

namespace Web::HTML {

class HTMLBaseElement final : public HTMLElement {
public:
    using WrapperType = Bindings::HTMLBaseElementWrapper;

    HTMLBaseElement(DOM::Document&, QualifiedName);
    virtual ~HTMLBaseElement() override;
};

}
