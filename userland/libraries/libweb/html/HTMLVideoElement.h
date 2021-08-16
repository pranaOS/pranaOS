/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libweb/html/HTMLMediaElement.h>

namespace Web::HTML {

class HTMLVideoElement final : public HTMLMediaElement {
public:
    using WrapperType = Bindings::HTMLVideoElementWrapper;

    HTMLVideoElement(DOM::Document&, QualifiedName);
    virtual ~HTMLVideoElement() override;
};

}
