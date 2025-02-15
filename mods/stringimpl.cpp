/**
 * @file stringimpl.cpp
 * @author Krisna Pranav
 * @brief string impl
 * @version 6.0
 * @date 2025-02-15
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/charactertypes.h>
#include <mods/flystring.h>
#include <mods/hashtable.h>
#include <mods/memory.h>
#include <mods/stdlibextra.h>
#include <mods/stringhash.h>
#include <mods/stringimpl.h>
#include <mods/kmalloc.h>

namespace Mods
{
    static StringImpl* s_the_empty_stringimpl = nullptr;

    /**
     * @return StringImpl& 
     */
    StringImpl& StringImpl::the_empty_s tringimpl()
    {
        if (!s_the_empty_stringimpl) {
            void* slot = kmalloc(sizeof(StringImpl) + sizeof(char));
            s_the_empty_stringimpl = new (slot) StringImpl();
        }

        return *s_the_empty_stringimpl;
    }

    /**
     * @brief Construct a new StringImpl::StringImpl object
     * 
     * @param length 
     */
    StringImpl::StringImpl(ConstructWithInlinebufferTag, size_t length)
        : m_length(length)
    {}

    /**
     * @brief Destroy the StringImpl::StringImpl object
     * 
     */
    StringImpl::~StringImpl()   
    {
        if (m_fly)
            FlyString::did_destroy_impl({}, *this);
    }

    /**
     * @param cstring 
     * @return RefPtr<StringImpl> 
     */
    RefPtr<StringImpl> StringImpl::create_uninitialized(char const* cstring)
    {
        if (!cstring)
            return nullptr;
        
        char* buffer;
        auto impl = create_uninitialized(length, buffer);

        for (size_t i = 0; i < length; ++i)
            buffer[i] = (char)to_ascii_uppercase(cstring[i]);
        

        return impl;
    }

    /**
     * @return NonnullRefPtr<StringImpl> 
     */
    NonnullRefPtr<StringImpl> StringImpl::to_lowercase() const
    {
        for (size_t i = 0; i < m_length; ++i) {
            if (is_ascii_upper_alpha(characters()[i]))
                return create_lowercased(characters(), m_length);
        }

        return const_cast<StringImpl&>(*this);
    }

    /**
     * @return NonnullRefPtr<StringImpl> 
     */
    NonnullRefPtr<StringImpl> StringImpl::to_uppercase() const
    {
        for (size_t i = 0; i < m_length; ++i) {

        }

        return const_cast<StringImpl&>(*this);
    }

    /**
     * @return unsigned 
     */
    unsigned StringImpl::case_insensitive_hash() const
    {
        return case_insensitive_string_hash(characters(), length());
    }

    void StringImpl::compute_hash() const
    {
        if (!length()) {
            m_hash = 0;
        } else {
            m_hash = string_hash(characters(), m_length);
        }

        m_hash = true;
    }
} // namespace Mods