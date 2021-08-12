/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Format.h>
#include <base/OwnPtr.h>

namespace Kernel {

class KString {
    BASE_MAKE_NONCOPYABLE(KString);
    BASE_MAKE_NONMOVABLE(KString);

public:
    static OwnPtr<KString> try_create_uninitialized(size_t, char*&);
    static NonnullOwnPtr<KString> must_create_uninitialized(size_t, char*&);
    static OwnPtr<KString> try_create(StringView const&);
    static NonnullOwnPtr<KString> must_create(StringView const&);

    void operator delete(void*);

    OwnPtr<KString> try_clone() const;

    bool is_empty() const { return m_length == 0; }
    size_t length() const { return m_length; }
    char const* characters() const { return m_characters; }
    StringView view() const { return { characters(), length() }; }

private:
    explicit KString(size_t length)
        : m_length(length)
    {
    }

    size_t m_length { 0 };
    char m_characters[0];
};

}

namespace Base {

template<>
struct Formatter<Kernel::KString> : Formatter<StringView> {
    void format(FormatBuilder& builder, Kernel::KString const& value)
    {
        Formatter<StringView>::format(builder, value.view());
    }
};

template<>
struct Formatter<OwnPtr<Kernel::KString>> : Formatter<StringView> {
    void format(FormatBuilder& builder, OwnPtr<Kernel::KString> const& value)
    {
        if (value)
            Formatter<StringView>::format(builder, value->view());
        else
            Formatter<StringView>::format(builder, "[out of memory]"sv);
    }
};

}
