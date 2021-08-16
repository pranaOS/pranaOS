/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/HashTable.h>
#include <libjs/runtime/Object.h>
#include <libjs/runtime/Set.h>

namespace JS {

class SetIterator final : public Object {
    JS_OBJECT(SetIterator, Object);

public:
    static SetIterator* create(GlobalObject&, Set& set, Object::PropertyKind iteration_kind);

    explicit SetIterator(Set& set, Object::PropertyKind iteration_kind, Object& prototype);
    virtual ~SetIterator() override;

    Set& set() const { return m_set; }
    bool done() const { return m_done; }
    Object::PropertyKind iteration_kind() const { return m_iteration_kind; }

private:
    friend class SetIteratorPrototype;

    virtual void visit_edges(Cell::Visitor&) override;

    Set& m_set;
    bool m_done { false };
    Object::PropertyKind m_iteration_kind;
    OrderedHashTable<Value, ValueTraits>::Iterator m_iterator;
};

}
