/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/SinglyLinkedList.h>
#include <libjs/runtime/FunctionObject.h>
#include <libjs/runtime/GlobalObject.h>
#include <libjs/runtime/Object.h>
#include <libjs/runtime/Value.h>
#include <libjs/runtime/WeakContainer.h>

namespace JS {

class FinalizationRegistry final
    : public Object
    , public WeakContainer {
    JS_OBJECT(FinalizationRegistry, Object);

public:
    static FinalizationRegistry* create(GlobalObject&, FunctionObject&);

    explicit FinalizationRegistry(FunctionObject&, Object& prototype);
    virtual ~FinalizationRegistry() override;

    void add_finalization_record(Cell& target, Value held_value, Object* unregister_token);
    bool remove_by_token(Object& unregister_token);
    void cleanup(FunctionObject* callback = nullptr);

    virtual void remove_swept_cells(Badge<Heap>, Vector<Cell*>&) override;

private:
    virtual void visit_edges(Visitor& visitor) override;

    FunctionObject* m_cleanup_callback { nullptr };

    struct FinalizationRecord {
        Cell* target { nullptr };
        Value held_value;
        Object* unregister_token { nullptr };
    };
    SinglyLinkedList<FinalizationRecord> m_records;
};

}
