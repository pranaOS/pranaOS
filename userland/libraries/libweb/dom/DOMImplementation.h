/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/NonnullRefPtr.h>
#include <base/RefCounted.h>
#include <base/Weakable.h>
#include <libweb/bindings/Wrappable.h>

namespace Web::DOM {

class DOMImplementation final
    : public RefCounted<DOMImplementation>
    , public Weakable<DOMImplementation>
    , public Bindings::Wrappable {
public:
    using WrapperType = Bindings::DOMImplementationWrapper;

    static NonnullRefPtr<DOMImplementation> create(Document& document)
    {
        return adopt_ref(*new DOMImplementation(document));
    }

    NonnullRefPtr<Document> create_document(const String&, const String&) const;
    NonnullRefPtr<Document> create_html_document(const String& title) const;
    NonnullRefPtr<DocumentType> create_document_type(const String&, const String&, const String&) const;

    bool has_feature() const { return true; }

private:
    explicit DOMImplementation(Document&);

    Document& m_document;
};

}
