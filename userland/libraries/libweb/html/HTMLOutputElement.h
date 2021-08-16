/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libweb/html/HTMLElement.h>

namespace Web::HTML {

class HTMLOutputElement final : public HTMLElement {
public:
    using WrapperType = Bindings::HTMLOutputElementWrapper;

    HTMLOutputElement(DOM::Document&, QualifiedName);
    virtual ~HTMLOutputElement() override;

    const String& type() const
    {
        static String output = "output";
        return output;
    }
};

}
