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
        static bool equals(StringImpl const* a, StringImpl const* b)
        {
            VERIFY(a);
            VERIFY(b);
            return *a == *b;
        }
    }; // struct FlyStringImplTraits : public Traits<StringImpl*> 

    static Singleton<HashTable<StringImpl*, FlyStringImplTraits>> s_table;

    /**
     * @return HashTable<StringImpl*, FlyStringImplTraits>& 
     */
    static HashTable<StringImpl*, FlyStringImplTraits>& fly_impls()
    {
        return *s_table;
    }

    /**
     * @param impl 
     */
    void FlyString::did_destroy_impl(Badge<StringImpl>, StringImpl& impl)
    {
        fly_impls().remove(&impl);
    }
    
    /**
     * @brief Construct a new FlyString::FlyString object
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

        if (it == fly_impls().end()) {
            fly_impls().set(const_cast<StringImpl*>(string.impl()));
            string.impl()->set_fly({}, true);
            m_impl = string.impl();
        } else {
            VERIFY((*it)->is_fly());
            m_impl = *it;
        }
    }

    /**
     * @brief Construct a new FlyString::FlyString object
     * 
     * @param string 
     */
    FlyString::FlyString(StringView string)
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
    
    /**
     * @tparam T 
     * @param trim_whitespace 
     * @return Optional<T> 
     */
    template<typename T>
    Optional<T> FlyString::to_int(TrimWhitespace trim_whitespace) const
    {
        return StringUtils::convert_to_int<T>(view(), trim_whitespace);
    }

    template Optional<i8> FlyString::to_int(TrimWhitespace) const;
    template Optional<i16> FlyString::to_int(TrimWhitespace) const;
    template Optional<i32> FlyString::to_int(TrimWhitespace) const;
    template Optional<i64> FlyString::to_int(TrimWhitespace) const;

    /**
     * @param trim_whitespace 
     * @return * template<typename T> 
     */
    template<typename T>
    Optional<T> FlyString::to_uint(TrimWhitespace trim_whitespace) const
    {
        return StringUtils::convert_to_uint<T>(view(), trim_whitespace);
    }

    template Optional<u8> FlyString::to_uint(TrimWhitespace) const;
    template Optional<u16> FlyString::to_uint(TrimWhitespace) const;
    template Optional<u32> FlyString::to_uint(TrimWhitespace) const;
    template Optional<u64> FlyString::to_uint(TrimWhitespace) const;

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool FlyString::equals_ignoring_case(StringView other) const
    {
        return StringUtils::equals_ignoring_case(view(), other);
    }

    /**
     * @param str 
     * @param case_sensitivity 
     * @return true 
     * @return false 
     */
    bool FlyString::starts_with(StringView str, CaseSensitivity case_sensitivity) const
    {
        return StringUtils::starts_with(view(), str, case_sensitivity);
    }

    /**
     * @param str 
     * @param case_sensitivity 
     * @return true 
     * @return false 
     */
    bool FlyString::ends_with(StringView str, CaseSensitivity case_sensitivity) const
    {
        return StringUtils::ends_with(view(), str, case_sensitivity);
    }

    /**
     * @return FlyString 
     */
    FlyString FlyString::to_lowercase() const
    {
        return String(*m_impl).to_lowercase();
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
    bool FlyString::operator==(char const* string) const
    {
        return view() == string;
    }
} // namespace Mods