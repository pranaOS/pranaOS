/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/heap/Heap.h>
#include <libjs/runtime/Symbol.h>
#include <libjs/runtime/VM.h>

namespace JS {

Symbol::Symbol(Optional<String> description, bool is_global)
    : m_description(move(description))
    , m_is_global(is_global)
{
}

Symbol::~Symbol()
{
}

Symbol* js_symbol(Heap& heap, Optional<String> description, bool is_global)
{
    return heap.allocate_without_global_object<Symbol>(move(description), is_global);
}

Symbol* js_symbol(VM& vm, Optional<String> description, bool is_global)
{
    return js_symbol(vm.heap(), move(description), is_global);
}

}
