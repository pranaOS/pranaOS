/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */


#pragma once

// includes
#include <base/HashMap.h>
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/Object.h>
#include <libjs/runtime/Value.h>

namespace JS {

class Map : public Object {
    JS_OBJECT(Map, Object);

public:
    static Map* create(GlobalObject&);

    explicit Map(Object& prototype);
    virtual ~Map() override;

    OrderedHashMap<Value, Value, ValueTraits> const& entries() const { return m_entries; };
    OrderedHashMap<Value, Value, ValueTraits>& entries() { return m_entries; };

private:
    virtual void visit_edges(Visitor& visitor) override;

    OrderedHashMap<Value, Value, ValueTraits> m_entries;
};

}
