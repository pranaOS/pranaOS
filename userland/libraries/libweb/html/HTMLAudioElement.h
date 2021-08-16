/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libweb/html/HTMLMediaElement.h>

namespace Web::HTML {

class HTMLAudioElement final : public HTMLMediaElement {
public:
    using WrapperType = Bindings::HTMLAudioElementWrapper;

    HTMLAudioElement(DOM::Document&, QualifiedName);
    virtual ~HTMLAudioElement() override;
};

}
