/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libweb/dom/Element.h>
#include <libweb/dom/HTMLCollection.h>
#include <libweb/dom/ParentNode.h>

namespace Web::DOM {

HTMLCollection::HTMLCollection(ParentNode& root, Function<bool(Element const&)> filter)
    : m_root(root)
    , m_filter(move(filter))
{
}

HTMLCollection::~HTMLCollection()
{
}

Vector<NonnullRefPtr<Element>> HTMLCollection::collect_matching_elements()
{
    Vector<NonnullRefPtr<Element>> elements;
    m_root->for_each_in_inclusive_subtree_of_type<Element>([&](auto& element) {
        if (m_filter(element))
            elements.append(element);
        return IterationDecision::Continue;
    });
    return elements;
}

size_t HTMLCollection::length()
{
    return collect_matching_elements().size();
}

Element* HTMLCollection::item(size_t index)
{
    auto elements = collect_matching_elements();
    if (index >= elements.size())
        return nullptr;
    return elements[index];
}

Element* HTMLCollection::named_item(FlyString const& name)
{
    if (name.is_null())
        return nullptr;
    auto elements = collect_matching_elements();

    if (auto it = elements.find_if([&](auto& entry) { return entry->attribute(HTML::AttributeNames::id) == name; }); it != elements.end())
        return *it;

    if (auto it = elements.find_if([&](auto& entry) { return entry->name() == name; }); it != elements.end())
        return *it;
    return nullptr;
}

}
