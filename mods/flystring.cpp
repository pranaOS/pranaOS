/**
 * @file flystring.cpp
 * @author Krisna Pranav
 * @brief flystring
 * @version 6.0
 * @date 2025-02-05
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/flystring.h>
#include <mods/hashtable.h>
#include <mods/optional.h>
#include <mods/singleton.h>
#include <mods/string.h>
#include <mods/stringutils.h>
#include <mods/stringview.h>

namespace Mods
{
    struct FlyStringImplTraits : public Traits<StringImpl*> 
    {
        /**
         * @param s 
         * @return unsigned 
         */
        static unsigned hash(StringImpl const* s) 
        {
            return s ? s->hash() : 0;
        }

        /**
         * @param a 
         * @param b 
         * @return true 
         * @return false 
         */
        static bool equals(StringImpl const* a, StringImpl* b)
        {
            VERIFY(a);
            VERIFY(b);

            return *a == *b;
        }
    }

    static Singleton<HashTable<StringImpl*, FlyStringImplTraits> s_table;

    /**
     * @param impl 
     */
    void FlyString::did_destroy_impl(Badge<StringImpl>, StringImpl& impl)
    {
        fly_impls().remove(&impl);
    }       
    
    /**
     * @brief Construct a new Fly String:: Fly String object
     * 
     * @param string 
     */
    FlyString::FlyString(String const& string)
    {
        if (string.is_null())
            return;
        
        if (string.impl()->is_fly()) {
            m_impl = string.impl();
            return;
        }

        auto it = fly_impls().find(const_cast<StringImpl*>(string.impl()));

        if(it == fly_impls().end()) {
            fly_impls().set(const_cast<StringImpl*>(string.impl()));
            string.impl()->set_fly({}, true);
            m_impl = string.impl();
        } else {
            VERIFY((*it)->is_fly());
            m_impl = *it;
        }
    }   

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool FlyString::operator==(String const& other) const
    {
        return m_impl == other.impl() || view() == other.view();
    }

    /**
     * @param string 
     * @return true 
     * @return false 
     */
    bool FlyString::operator==(StringView string) const
    {
        return view() == string;
    }

    /**
     * @param string 
     * @return true 
     * @return false 
     */
    bool FlyString::operator==(const char* string) const
    {
        return view() == string;
    }
} // namespace Mods