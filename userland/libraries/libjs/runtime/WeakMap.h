/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// incldues
#include <base/HashMap.h>
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/Object.h>
#include <libjs/runtime/WeakContainer.h>

namespace JS {

class WeakMap final
    : public Object
    , public WeakContainer {
    JS_OBJECT(WeakMap, Object);

public:
    static WeakMap* create(GlobalObject&);

    explicit WeakMap(Object& prototype);
    virtual ~WeakMap() override;

    HashMap<Cell*, Value> const& values() const { return m_values; };
    HashMap<Cell*, Value>& values() { return m_values; };

    virtual void remove_swept_cells(Badge<Heap>, Vector<Cell*>&) override;

private:
    HashMap<Cell*, Value> m_values; 
};

}
