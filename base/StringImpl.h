/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Badge.h>
#include <base/RefCounted.h>
#include <base/RefPtr.h>
#include <base/Span.h>
#include <base/Types.h>
#include <base/kmalloc.h>

namespace Base {

enum ShouldChomp {
    NoChomp,
    Chomp
};

class StringImpl : public RefCounted<StringImpl> {
public:
    static NonnullRefPtr<StringImpl> create_uninitialized(size_t length, char*& buffer);
    static RefPtr<StringImpl> create(const char* cstring, ShouldChomp = NoChomp);
    static RefPtr<StringImpl> create(const char* cstring, size_t length, ShouldChomp = NoChomp);
    static RefPtr<StringImpl> create(ReadonlyBytes, ShouldChomp = NoChomp);
    static RefPtr<StringImpl> create_lowercased(char const* cstring, size_t length);
    static RefPtr<StringImpl> create_uppercased(char const* cstring, size_t length);

    NonnullRefPtr<StringImpl> to_lowercase() const;
    NonnullRefPtr<StringImpl> to_uppercase() const;

    void operator delete(void* ptr)
    {
        kfree_sized(ptr, allocation_size_for_stringimpl(static_cast<StringImpl*>(ptr)->m_length));
    }

    static StringImpl& the_empty_stringimpl();

    ~StringImpl();

    size_t length() const { return length; }

};

}