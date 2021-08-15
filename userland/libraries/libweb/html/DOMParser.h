/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/RefCounted.h>
#include <base/Weakable.h>
#include <libweb/bindings/Wrappable.h>
#include <libweb/dom/Document.h>
#include <libweb/dom/ExceptionOr.h>
#include <libweb/Forward.h>

namespace Web::HTML {

class DOMParser final
    : public RefCounted<DOMParser>
    , public Weakable<DOMParser>
    , public Bindings::Wrappable {
public:
    using WrapperType = Bindings::DOMParserWrapper;

    static DOM::ExceptionOr<NonnullRefPtr<DOMParser>> create_with_global_object(Bindings::WindowObject&)
    {
        return adopt_ref(*new DOMParser());
    }

    virtual ~DOMParser() override;

    NonnullRefPtr<DOM::Document> parse_from_string(String const&, String const&);

private:
    DOMParser();
};

}
