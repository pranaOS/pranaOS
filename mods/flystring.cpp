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

#include <mods/deprecatedflystring.h>
#include <mods/flystring.h>
#include <mods/hashmap.h>
#include <mods/singleton.h>
#include <mods/string.h>
#include <mods/stringdata.h>
#include <mods/stringview.h>
#include <mods/utf8view.h>

namespace Mods 
{

    struct FlyStringTableHashTraits : public Traits<Detail::StringData const*> 
    {
        /**
         * @param string 
         * @return u32 
         */
        static u32 hash(Detail::StringData const* string) 
        { 
            return string->hash(); 
        }

        /**
         * @param a 
         * @param b 
         * @return true 
         * @return false 
         */
        static bool equals(Detail::StringData const* a, Detail::StringData const* b) 
        { 
            return *a == *b; 
        }
    }; // struct FlyStringTableHashTraits : public Traits<Detail::StringData const*> 

    static auto& all_fly_strings()
    {
        static Singleton<HashTable<Detail::StringData const*, FlyStringTableHashTraits>> table;
        return *table;
    }

    /**
     * @param string 
     * @return ErrorOr<FlyString> 
     */
    ErrorOr<FlyString> FlyString::from_utf8(StringView string)
    {
        if (string.is_empty())
            return FlyString {};

        if (string.length() <= Detail::MAX_SHORT_STRING_BYTE_COUNT)
            return FlyString { TRY(String::from_utf8(string)) };

        if (auto it = all_fly_strings().find(string.hash(), [&](auto& entry) { return entry->bytes_as_string_view() == string; }); it != all_fly_strings().end())
            return FlyString { Detail::StringBase(**it) };

        return FlyString { TRY(String::from_utf8(string)) };
    }

    /**
     * @param string 
     * @return FlyString 
     */
    FlyString FlyString::from_utf8_without_validation(ReadonlyBytes string)
    {
        if (string.is_empty())
            return FlyString {};

        if (string.size() <= Detail::MAX_SHORT_STRING_BYTE_COUNT)
            return FlyString { String::from_utf8_without_validation(string) };
            
        if (auto it = all_fly_strings().find(StringView(string).hash(), [&](auto& entry) { return entry->bytes_as_string_view() == string; }); it != all_fly_strings().end())
            return FlyString { Detail::StringBase(**it) };

        return FlyString { String::from_utf8_without_validation(string) };
    }

    /**
     * @brief Construct a new Fly String:: Fly String object
     * 
     * @param string 
     */
    FlyString::FlyString(String const& string)
    {
        if (string.is_short_string()) {
            m_data = string;
            return;
        }

        if (string.m_data->is_fly_string()) {
            m_data = string;
            return;
        }

        auto it = all_fly_strings().find(string.m_data);

        if (it == all_fly_strings().end()) {
            m_data = string;
            all_fly_strings().set(string.m_data);
            string.m_data->set_fly_string(true);
        } else {
            m_data.m_data = *it;
            m_data.m_data->ref();
        }
    }

    /**
     * @param string 
     * @return FlyString& 
     */
    FlyString& FlyString::operator=(String const& string)
    {
        *this = FlyString { string };
        return *this;
    }

    /**
     * @return true 
     * @return false 
     */
    bool FlyString::is_empty() const
    {
        return bytes_as_string_view().is_empty();
    }

    /**
     * @return unsigned 
     */
    unsigned FlyString::hash() const
    {
        return m_data.hash();
    }

    /**
     * @return u32 
     */
    u32 FlyString::ascii_case_insensitive_hash() const
    {
        return case_insensitive_string_hash(reinterpret_cast<char const*>(bytes().data()), bytes().size());
    }

    /**
     * @return String 
     */
    FlyString::operator String() const
    {
        return to_string();
    }

    /**
     * @return String 
     */
    String FlyString::to_string() const
    {
        Detail::StringBase copy = m_data;
        return String(move(copy));
    }

    /**
     * @return Utf8View 
     */
    Utf8View FlyString::code_points() const
    {
        return Utf8View { bytes_as_string_view() };
    }

    /**
     * @return ReadonlyBytes 
     */
    ReadonlyBytes FlyString::bytes() const
    {
        return bytes_as_string_view().bytes();
    }

    /**
     * @return StringView 
     */
    StringView FlyString::bytes_as_string_view() const
    {
        return m_data.bytes();
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool FlyString::operator==(String const& other) const
    {
        return m_data == other;
    }

    /**
     * @param string 
     * @return true 
     * @return false 
     */
    bool FlyString::operator==(StringView string) const
    {
        return bytes_as_string_view() == string;
    }

    /**
     * @param string 
     * @return true 
     * @return false 
     */
    bool FlyString::operator==(char const* string) const
    {
        return bytes_as_string_view() == string;
    }

    /**
     * @param string_data 
     */
    void FlyString::did_destroy_fly_string_data(Badge<Detail::StringData>, Detail::StringData const& string_data)
    {
        all_fly_strings().remove(&string_data);
    }

    /**
     * @return Detail::StringBase 
     */
    Detail::StringBase FlyString::data(Badge<String>) const
    {
        return m_data;
    }

    /**
     * @return size_t 
     */
    size_t FlyString::number_of_fly_strings()
    {
        return all_fly_strings().size();
    }

    /**
     * @return DeprecatedFlyString 
     */
    DeprecatedFlyString FlyString::to_deprecated_fly_string() const
    {
        return DeprecatedFlyString(bytes_as_string_view());
    }

    /**
     * @param deprecated_fly_string 
     * @return ErrorOr<FlyString> 
     */
    ErrorOr<FlyString> FlyString::from_deprecated_fly_string(DeprecatedFlyString const& deprecated_fly_string)
    {
        return FlyString::from_utf8(deprecated_fly_string.view());
    }

    /**
     * @param fly_string 
     * @return unsigned 
     */
    unsigned Traits<FlyString>::hash(FlyString const& fly_string)
    {
        return fly_string.hash();
    }

    /**
     * @param other
     */
    int FlyString::operator<=>(FlyString const& other) const
    {
        return bytes_as_string_view().compare(other.bytes_as_string_view());
    }

    /**
     * @param builder 
     * @param fly_string 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> Formatter<FlyString>::format(FormatBuilder& builder, FlyString const& fly_string)
    {
        return Formatter<StringView>::format(builder, fly_string.bytes_as_string_view());
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool FlyString::equals_ignoring_ascii_case(FlyString const& other) const
    {
        if (*this == other)
            return true;

        return StringUtils::equals_ignoring_ascii_case(bytes_as_string_view(), other.bytes_as_string_view());
    }

    /**
     * @param other 
     * @return true 
     * @return false 
     */
    bool FlyString::equals_ignoring_ascii_case(StringView other) const
    {
        return StringUtils::equals_ignoring_ascii_case(bytes_as_string_view(), other);
    }

    /**
     * @param bytes 
     * @param case_sensitivity 
     * @return true 
     * @return false 
     */
    bool FlyString::starts_with_bytes(StringView bytes, CaseSensitivity case_sensitivity) const
    {
        return bytes_as_string_view().starts_with(bytes, case_sensitivity);
    }

    /**
     * @param bytes 
     * @param case_sensitivity 
     * @return true 
     * @return false 
     */
    bool FlyString::ends_with_bytes(StringView bytes, CaseSensitivity case_sensitivity) const
    {
        return bytes_as_string_view().ends_with(bytes, case_sensitivity);
    }

} // namespace Mods
