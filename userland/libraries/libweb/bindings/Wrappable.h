/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/WeakPtr.h>
#include <libjs/heap/Heap.h>
#include <libjs/runtime/GlobalObject.h>
#include <libweb/Forward.h>

namespace Web::Bindings {

class Wrappable {
public:
    virtual ~Wrappable();

    void set_wrapper(Wrapper&);
    Wrapper* wrapper() { return m_wrapper; }
    const Wrapper* wrapper() const { return m_wrapper; }

private:
    WeakPtr<Wrapper> m_wrapper;
};

template<class NativeObject>
inline Wrapper* wrap_impl(JS::GlobalObject& global_object, NativeObject& native_object)
{
    if (!native_object.wrapper()) {
        native_object.set_wrapper(*global_object.heap().allocate<typename NativeObject::WrapperType>(global_object, global_object, native_object));
    }
    return native_object.wrapper();
}

}
