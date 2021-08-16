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
#include <libjs/runtime/WeakContainer.h>

namespace JS {

class WeakSet final
    : public Object
    , public WeakContainer {
    JS_OBJECT(WeakSet, Object);

public:
    static WeakSet* create(GlobalObject&);

    explicit WeakSet(Object& prototype);
    virtual ~WeakSet() override;

    HashTable<Cell*> const& values() const { return m_values; };
    HashTable<Cell*>& values() { return m_values; };

    virtual void remove_swept_cells(Badge<Heap>, Vector<Cell*>&) override;

private:
    HashTable<Cell*> m_values; 
};

}
