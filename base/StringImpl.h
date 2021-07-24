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

       const char* characters() const { return &m_inline_buffer[0]; }

    ALWAYS_INLINE ReadonlyBytes bytes() const { return { characters(), length() }; }
    ALWAYS_INLINE StringView view() const { return { characters(), length() }; }

    const char& operator[](size_t i) const
    {
        VERIFY(i < m_length);
        return characters()[i];
    }

    bool operator==(const StringImpl& other) const
    {
        if (length() != other.length())
            return false;
        return !__builtin_memcmp(characters(), other.characters(), length());
    }

    unsigned hash() const
    {
        if (!m_has_hash)
            compute_hash();
        return m_hash;
    }

    unsigned existing_hash() const
    {
        return m_hash;
    }

    bool is_fly() const { return m_fly; }
    void set_fly(Badge<FlyString>, bool fly) const { m_fly = fly; }

private:
    enum ConstructTheEmptyStringImplTag {
        ConstructTheEmptyStringImpl
    };
    explicit StringImpl(ConstructTheEmptyStringImplTag)
        : m_fly(true)
    {
        m_inline_buffer[0] = '\0';
    }

};

}