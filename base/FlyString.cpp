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


}