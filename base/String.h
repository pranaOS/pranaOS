/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Format.h>
#include <base/Forward.h>
#include <base/RefPtr.h>
#include <base/Stream.h>
#include <base/StringBuilder.h>
#include <base/StringImpl.h>
#include <base/StringUtils.h>
#include <base/Traits.h>

namespace Base {

class String {
public:
    ~String() = default;

    String() = default;

    String(const StringView& view)
    {
        m_impl = StringImpl::create(view.characters_without_null_termination(), view.length());
    }

    String(const String& other)
        : m_impl(const_cast<String&>(other).m_impl)
    {
    }

    String(String&& other)
        : m_impl(move(other.m_impl))
    {
    }

    String(const char* cstring, ShouldChomp shouldChomp = NoChomp)
        : m_impl(StringImpl::create(cstring, shouldChomp))
    {
    }

    String(const char* cstring, size_t length, ShouldChomp shouldChomp = NoChomp)
        : m_impl(StringImpl::create(cstring, length, shouldChomp))
    {
    }

    explicit String(ReadonlyBytes bytes, ShouldChomp shouldChomp = NoChomp)
        : m_impl(StringImpl::create(bytes, shouldChomp))
    {
    }

    String(const StringImpl& impl)
        : m_impl(const_cast<StringImpl&>(impl))
    {
    }

    String(const StringImpl* impl)
        : m_impl(const_cast<StringImpl*>(impl))
    {
    }

    String(RefPtr<StringImpl>&& impl)
        : m_impl(move(impl))
    {
    }

    String(NonnullRefPtr<StringImpl>&& impl)
        : m_impl(move(impl))
    {
    }

        String(const FlyString&);

    [[nodiscard]] static String repeated(char, size_t count);
    [[nodiscard]] static String repeated(const StringView&, size_t count);

    [[nodiscard]] static String bijective_base_from(size_t value, unsigned base = 26, StringView map = {});
    [[nodiscard]] static String roman_number_from(size_t value);

    template<class SeparatorType, class CollectionType>
    [[nodiscard]] static String join(const SeparatorType& separator, const CollectionType& collection)
    {
        StringBuilder builder;
        builder.join(separator, collection);
        return builder.build();
    }

    [[nodiscard]] bool matches(const StringView& mask, CaseSensitivity = CaseSensitivity::CaseInsensitive) const;
    [[nodiscard]] bool matches(const StringView& mask, Vector<MaskSpan>&, CaseSensitivity = CaseSensitivity::CaseInsensitive) const;

    template<typename T = int>
    [[nodiscard]] Optional<T> to_int(TrimWhitespace = TrimWhitespace::Yes) const;
    template<typename T = unsigned>
    [[nodiscard]] Optional<T> to_uint(TrimWhitespace = TrimWhitespace::Yes) const;

    [[nodiscard]] String to_lowercase() const;
    [[nodiscard]] String to_uppercase() const;
    [[nodiscard]] String to_snakecase() const;

    [[nodiscard]] bool is_whitespace() const { return StringUtils::is_whitespace(*this); }

};

}
