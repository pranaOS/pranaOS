/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#include <base/CharacterTypes.h>
#include <base/FlyString.h>
#include <base/HashTable.h>
#include <base/Memory.h>
#include <base/StdLibExtras.h>
#include <base/StringHash.h>
#include <base/StringImpl.h>
#include <base/kmalloc.h>


namespace Base {

StringImpl& StringImpl::the_empty_stringimpl()
{
    if (!s_the_empty_stringimpl) {
        void* slot = kmalloc(sizeof(StringImpl) + sizeof(char));
        s_the_empty_stringimpl = new (slot) StringImpl(ConstructTheEmptyStringImpl);
    }
    return *s_the_empty_stringimpl;
}

StringImpl::StringImpl(ConstructWithInlineBufferTag, size_t length)
    : m_length(length)
{
}

StringImpl::~StringImpl()
{
    if (m_fly)
        FlyString::did_destroy_impl({}, *this);
}

NonnullRefPtr<StringImpl> StringImpl::create_uninitialized(size_t length, char*& buffer)
{
    VERIFY(length);
    void* slot = kmalloc(allocation_size_for_stringimpl(length));
    VERIFY(slot);
    auto new_stringimpl = adopt_ref(*new (slot) StringImpl(ConstructWithInlineBuffer, length));
    buffer = const_cast<char*>(new_stringimpl->characters());
    buffer[length] = '\0';
    return new_stringimpl;
}

RefPtr<StringImpl> StringImpl::create(const char* cstring, size_t length, ShouldChomp should_chomp)
{
    if (!cstring)
        return nullptr;

    if (should_chomp) {
        while (length) {
            char last_ch = cstring[length - 1];
            if (!last_ch || last_ch == '\n' || last_ch == '\r')
                --length;
            else
                break;
        }
    }

    if (!length)
        return the_empty_stringimpl();

    char* buffer;
    auto new_stringimpl = create_uninitialized(length, buffer);
    memcpy(buffer, cstring, length * sizeof(char));

    return new_stringimpl;
}

RefPtr<StringImpl> StringImpl::create(const char* cstring, ShouldChomp shouldChomp)
{
    if (!cstring)
        return nullptr;

    return create(cstring, strlen(cstring), shouldChomp);
}

}