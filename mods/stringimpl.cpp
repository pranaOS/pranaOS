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