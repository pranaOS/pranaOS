/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#include <base/FlyString.h>
#include <base/HashTable.h>
#include <base/Optional.h>
#include <base/Singleton.h>
#include <base/String.h>
#include <base/StringUtils.h>
#include <base/StringView.h>


namespace Base {

struct FlyStringImplTraits : public Traits<StringImpl*> {
    static unsigned hash(const StringImpl* s) { return s ? s->hash() : 0; }
    static bool equals(const StringImpl* a, const StringImpl* b)
    {
        VERIFY(a);
        VERIFY(b);
        return *a == *b;
    }
};

static Base::Singleton<HashTable<StringImpl*, FlyStringImplTraits>> s_table;

static HashTable<StringImpl*, FlyStringImplTraits>& fly_impls()
{
    return *s_table;
}

void FlyString::did_destroy_impl(Badge<StringImpl>, StringImpl& impl)
{
    fly_impls().remove(&impl);
}


FlyString::FlyString(const String& string)
{
    if (string.is_null())
        return;
    if (string.impl()->is_fly()) {
        m_impl = string.impl();
        return;
    }
    auto it = fly_impls().find(const_cast<StringImpl*>(string.impl()));
    if (it == fly_impls().end()) {
        fly_impls().set(const_cast<StringImpl*>(string.impl()));
        string.impl()->set_fly({}, true);
        m_impl = string.impl();
    } else {
        VERIFY((*it)->is_fly());
        m_impl = *it;
    }
}

FlyString::FlyString(StringView const& string)
{
    if (string.is_null())
        return;
    auto it = fly_impls().find(string.hash(), [&](auto& candidate) {
        return string == candidate;
    });
    if (it == fly_impls().end()) {
        auto new_string = string.to_string();
        fly_impls().set(new_string.impl());
        new_string.impl()->set_fly({}, true);
        m_impl = new_string.impl();
    } else {
        VERIFY((*it)->is_fly());
        m_impl = *it;
    }
}


}