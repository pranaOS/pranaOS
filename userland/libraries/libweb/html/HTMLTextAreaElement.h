/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libweb/html/HTMLElement.h>

namespace Web::HTML {

class HTMLTextAreaElement final : public HTMLElement {
public:
    using WrapperType = Bindings::HTMLTextAreaElementWrapper;

    HTMLTextAreaElement(DOM::Document&, QualifiedName);
    virtual ~HTMLTextAreaElement() override;

    const String& type() const
    {
        static String textarea = "textarea";
        return textarea;
    }
};

}
