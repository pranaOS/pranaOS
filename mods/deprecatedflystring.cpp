/**
 * @file deprecatedflystring.cpp
 * @author Krisna Pranav
 * @brief deprecated fly string
 * @version 6.0
 * @date 2025-03-16
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/bytestring.h>
#include <mods/deprecatedflystring.h>
#include <mods/hashtable.h>
#include <mods/optional.h>
#include <mods/singleton.h>
#include <mods/stringutils.h>
#include <mods/stringview.h>

namespace Mods 
{

    struct DeprecatedFlyStringImplTraits : public Traits<StringImpl const*> {

        /**
         * @param s 
         * @return unsigned 
         */
        static unsigned hash(StringImpl const* s) 
        { 
            return s->hash(); 
        }

        /**
         * @param a 
         * @param b 
         * @return true 
         * @return false 
         */
        static bool equals(StringImpl const* a, StringImpl const* b)
        {
            return *a == *b;
        }
    }; // struct DeprecatedFlyStringImplTraits : public Traits<StringImpl const*>

    static Singleton<HashTable<StringImpl const*, DeprecatedFlyStringImplTraits>> s_table;

    /**
     * @return HashTable<StringImpl const*, DeprecatedFlyStringImplTraits>& 
     */
    static HashTable<StringImpl const*, DeprecatedFlyStringImplTraits>& fly_impls()
    {
        return *s_table;
    }

    /**
     * @param impl 
     */
    void DeprecatedFlyString::did_destroy_impl(Badge<StringImpl>, StringImpl& impl)
    {
        fly_impls().remove(&impl);
    }

    /**
     * @brief Construct a new DeprecatedFlyString::DeprecatedFlyString object
     * 
     * @param string 
     */
    DeprecatedFlyString::DeprecatedFlyString(ByteString const& string)
        : m_impl(string.impl())
    {
        if (string.impl()->is_fly())
            return;

        auto it = fly_impls().find(string.impl());
        if (it == fly_impls().end()) {
            fly_impls().set(string.impl());
            string.impl()->set_fly({}, true);
            m_impl = string.impl();
        } else {
            VERIFY((*it)->is_fly());
            m_impl = **it;
        }
    }

    /**
     * @brief Construct a new DeprecatedFlyString::DeprecatedFlyString object
     * 
     * @param string 
     */
    DeprecatedFlyString::DeprecatedFlyString(StringView string)
        : m_impl(StringImpl::the_empty_stringimpl())
    {
        if (string.is_null())
            return;
        auto it = fly_impls().find(string.hash(), [&](auto& candidate) {
            return string == *candidate;
        });
        if (it == fly_impls().end()) {
            auto new_string = string.to_byte_string();
            fly_impls().set(new_string.impl());
            new_string.impl()->set_fly({}, true);
            m_impl = new_string.impl();
        } else {
            VERIFY((*it)->is_fly());
            m_impl = **it;
        }
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool DeprecatedFlyString::equals_ignoring_ascii_case(StringView other) const
    {
        return StringUtils::equals_ignoring_ascii_case(view(), other);
    }

    /**
     * @param str 
     * @param case_sensitivity 
     * @return true 
     * @return false 
     */
    bool DeprecatedFlyString::starts_with(StringView str, CaseSensitivity case_sensitivity) const
    {
        return StringUtils::starts_with(view(), str, case_sensitivity);
    }

    /**
     * @param str 
     * @param case_sensitivity 
     * @return true 
     * @return false 
     */
    bool DeprecatedFlyString::ends_with(StringView str, CaseSensitivity case_sensitivity) const
    {
        return StringUtils::ends_with(view(), str, case_sensitivity);
    }

    /**
     * @return DeprecatedFlyString 
     */
    DeprecatedFlyString DeprecatedFlyString::to_lowercase() const
    {
        return ByteString(*m_impl).to_lowercase();
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool DeprecatedFlyString::operator==(ByteString const& other) const
    {
        return m_impl == other.impl() || view() == other.view();
    }

    /**
     * @param string 
     * @return true 
     * @return false 
     */
    bool DeprecatedFlyString::operator==(StringView string) const
    {
        return view() == string;
    }

    /**
     * @param string 
     * @return true 
     * @return false 
     */
    bool DeprecatedFlyString::operator==(char const* string) const
    {
        return view() == string;
    }

} // namespace Mods
