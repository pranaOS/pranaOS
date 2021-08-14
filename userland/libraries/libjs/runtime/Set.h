/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/HashTable.h>
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/Object.h>
#include <libjs/runtime/Value.h>

namespace JS {

class Set : public Object {
    JS_OBJECT(Set, Object);

public:
    static Set* create(GlobalObject&);

    explicit Set(Object& prototype);
    virtual ~Set() override;

    OrderedHashTable<Value, ValueTraits> const& values() const { return m_values; };
    OrderedHashTable<Value, ValueTraits>& values() { return m_values; };

private:
    virtual void visit_edges(Visitor& visitor) override;

    OrderedHashTable<Value, ValueTraits> m_values;
};

}
