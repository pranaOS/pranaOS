/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/FlyString.h>
#include <base/Function.h>
#include <base/Noncopyable.h>
#include <libweb/bindings/Wrappable.h>
#include <libweb/Forward.h>

namespace Web::DOM {

class HTMLCollection
    : public RefCounted<HTMLCollection>
    , public Bindings::Wrappable {
    BASE_MAKE_NONCOPYABLE(HTMLCollection);
    BASE_MAKE_NONMOVABLE(HTMLCollection);

public:
    using WrapperType = Bindings::HTMLCollectionWrapper;

    static NonnullRefPtr<HTMLCollection> create(ParentNode& root, Function<bool(Element const&)> filter)
    {
        return adopt_ref(*new HTMLCollection(root, move(filter)));
    }

    ~HTMLCollection();

    size_t length();
    Element* item(size_t index);
    Element* named_item(FlyString const& name);

    Vector<NonnullRefPtr<Element>> collect_matching_elements();

protected:
    HTMLCollection(ParentNode& root, Function<bool(Element const&)> filter);

private:
    NonnullRefPtr<ParentNode> m_root;
    Function<bool(Element const&)> m_filter;
};

}

namespace Web::Bindings {

HTMLCollectionWrapper* wrap(JS::GlobalObject&, DOM::HTMLCollection&);

}
