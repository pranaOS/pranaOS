/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/Utf8View.h>
#include <libjs/runtime/Object.h>

namespace JS {

class StringIterator final : public Object {
    JS_OBJECT(StringIterator, Object);

public:
    static StringIterator* create(GlobalObject&, String string);

    explicit StringIterator(String string, Object& prototype);
    virtual ~StringIterator() override;

    Utf8CodePointIterator& iterator() { return m_iterator; }
    bool done() const { return m_done; }

private:
    friend class StringIteratorPrototype;

    String m_string;
    Utf8CodePointIterator m_iterator;
    bool m_done { false };
};

}
