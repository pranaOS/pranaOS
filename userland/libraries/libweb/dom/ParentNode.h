/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/NonnullRefPtrVector.h>
#include <libweb/dom/Node.h>

namespace Web::DOM {

class ParentNode : public Node {
public:
    template<typename F>
    void for_each_child(F) const;
    template<typename F>
    void for_each_child(F);

    RefPtr<Element> first_element_child();
    RefPtr<Element> last_element_child();
    u32 child_element_count() const;

    RefPtr<Element> query_selector(const StringView&);
    NonnullRefPtrVector<Element> query_selector_all(const StringView&);

protected:
    ParentNode(Document& document, NodeType type)
        : Node(document, type)
    {
    }
};

template<typename Callback>
inline void ParentNode::for_each_child(Callback callback) const
{
    for (auto* node = first_child(); node; node = node->next_sibling())
        callback(*node);
}

template<typename Callback>
inline void ParentNode::for_each_child(Callback callback)
{
    for (auto* node = first_child(); node; node = node->next_sibling())
        callback(*node);
}

}
