/**
 * @file locale.h
 * @author Krisna Pranav
 * @brief Locale
 * @version 6.0
 * @date 2024-11-06
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/charactertypes.h>
#include <mods/optional.h>
#include <mods/string.h>
#include <mods/string_view.h>
#include <mods/variant.h>
#include <mods/vector.h>
#include <libunicode/forward.h>

namespace Unicode 
{

    struct LanguageID 
    {
        String to_string() const;

        /**
         * @return true 
         * @return false 
         */
        bool operator==(LanguageID const&) const = default;

        bool is_root { false };
        Optional<String> language {};
        Optional<String> script {};
        Optional<String> region {};
        Vector<String> variants {};
    }; // struct LanguageID 

    struct Keyword 
    {
        String key {};
        String value {};
    }; // struct Keyword 

    struct LocaleExtension 
    {
        Vector<String> attributes {};
        Vector<Keyword> keywords {};
    }; // struct LocaleExtension 

    struct TransformedField 
    {
        String key {};
        String value {};
    }; // struct TransformedField 

    struct TransformedExtension 
    {
        Optional<LanguageID> language {};
        Vector<TransformedField> fields {};
    }; // struct TransformedExtension 

    struct OtherExtension 
    {
        char key {};
        String value {};
    }; // struct OtherExtension 

    using Extension = Variant<LocaleExtension, TransformedExtension, OtherExtension>;

    struct LocaleID 
    {
        String to_string() const;

        /**
         * @tparam ExtensionType 
         * @return Vector<Extension> 
         */
        template<typename ExtensionType>
        Vector<Extension> remove_extension_type()
        {
            Vector<Extension> removed_extensions {};
            auto tmp_extensions = move(extensions);

            for (auto& extension : tmp_extensions) {
                if (extension.has<ExtensionType>())
                    removed_extensions.append(move(extension));
                else
                    extensions.append(move(extension));
            }

            return removed_extensions;
        }

        LanguageID language_id {};
        Vector<Extension> extensions {};
        Vector<String> private_use_extensions {};
    }; // struct LocaleID 

    enum class Style : u8 
    {
        Long,
        Short,
        Narrow,
    }; // enum class Style : u8 

    struct DisplayPattern 
    {
        StringView locale_pattern;
        StringView locale_separator;
    }; // struct DisplayPattern 

    struct ListPatterns 
    {
        StringView start;
        StringView middle;
        StringView end;
        StringView pair;
    }; // struct ListPatterns 

    /**
     * @param subtag 
     * @return true 
     * @return false 
     */
    constexpr bool is_unicode_language_subtag(StringView subtag)
    {
        if ((subtag.length() < 2) || (subtag.length() == 4) || (subtag.length() > 8))
            return false;
        return all_of(subtag, is_ascii_alpha);
    }

    /**
     * @param subtag 
     * @return true 
     * @return false 
     */
    constexpr bool is_unicode_script_subtag(StringView subtag)
    {
        if (subtag.length() != 4)
            return false;
        return all_of(subtag, is_ascii_alpha);
    }

    /**
     * @param subtag 
     * @return true 
     * @return false 
     */
    constexpr bool is_unicode_region_subtag(StringView subtag)
    {
        if (subtag.length() == 2)
            return all_of(subtag, is_ascii_alpha);

        if (subtag.length() == 3)
            return all_of(subtag, is_ascii_digit);

        return false;
    }

    /**
     * @param subtag 
     * @return true 
     * @return false 
     */
    constexpr bool is_unicode_variant_subtag(StringView subtag)
    {
        if ((subtag.length() >= 5) && (subtag.length() <= 8))
            return all_of(subtag, is_ascii_alphanumeric);
        if (subtag.length() == 4)
            return is_ascii_digit(subtag[0]) && all_of(subtag.substring_view(1), is_ascii_alphanumeric);
        return false;
    }

    bool is_type_identifier(StringView);

    Optional<LanguageID> parse_unicode_language_id(StringView);
    Optional<LocaleID> parse_unicode_locale_id(StringView);

    /**
     * @param key 
     * @param value 
     * @param remove_true 
     */
    void canonicalize_unicode_extension_values(StringView key, String& value, bool remove_true);
    
    Optional<String> canonicalize_unicode_locale_id(LocaleID&);

    String const& default_locale();

    /**
     * @param locale 
     * @return true 
     * @return false 
     */
    bool is_locale_available(StringView locale);

    Span<StringView const> get_available_calendars();
    Span<StringView const> get_available_number_systems();

    /**
     * @param style 
     * @return Style 
     */
    Style style_from_string(StringView style);

    /**
     * @param style 
     * @return StringView 
     */
    StringView style_to_string(Style style);

    /**
     * @param locale 
     * @return Optional<Locale> 
     */
    Optional<Locale> locale_from_string(StringView locale);

    /**
     * @param language 
     * @return Optional<Language> 
     */
    Optional<Language> language_from_string(StringView language);

    /**
     * @param territory 
     * @return Optional<Territory> 
     */
    Optional<Territory> territory_from_string(StringView territory);

    /**
     * @param script_tag 
     * @return Optional<ScriptTag> 
     */
    Optional<ScriptTag> script_tag_from_string(StringView script_tag);

    /**
     * @param currency 
     * @return Optional<Currency> 
     */
    Optional<Currency> currency_from_string(StringView currency);

    /**
     * @param calendar 
     * @return Optional<DateField> 
     */
    Optional<DateField> date_field_from_string(StringView calendar);

    /**
     * @param list_pattern_type 
     * @return Optional<ListPatternType> 
     */
    Optional<ListPatternType> list_pattern_type_from_string(StringView list_pattern_type);

    /**
     * @param key 
     * @return Optional<Key> 
     */
    Optional<Key> key_from_string(StringView key);

    /**
     * @param ca 
     * @return Optional<KeywordCalendar> 
     */
    Optional<KeywordCalendar> keyword_ca_from_string(StringView ca);

    /**
     * @param kf 
     * @return Optional<KeywordColCaseFirst> 
     */
    Optional<KeywordColCaseFirst> keyword_kf_from_string(StringView kf);

    /**
     * @param kn 
     * @return Optional<KeywordColNumeric> 
     */
    Optional<KeywordColNumeric> keyword_kn_from_string(StringView kn);

    /**
     * @param nu 
     * @return Optional<KeywordNumbers> 
     */
    Optional<KeywordNumbers> keyword_nu_from_string(StringView nu);

    /**
     * @brief Get the keywords for locale object
     * 
     * @param locale 
     * @param key 
     * @return Vector<StringView> 
     */
    Vector<StringView> get_keywords_for_locale(StringView locale, StringView key);

    /**
     * @brief Get the locale display patterns object
     * 
     * @param locale 
     * @return Optional<DisplayPattern> 
     */
    Optional<DisplayPattern> get_locale_display_patterns(StringView locale);

    /**
     * @param locale 
     * @param locale_id 
     * @return Optional<String> 
     */
    Optional<String> format_locale_for_display(StringView locale, LocaleID locale_id);

    /**
     * @brief Get the locale language mapping object
     * 
     * @param locale 
     * @param language 
     * @return Optional<StringView> 
     */
    Optional<StringView> get_locale_language_mapping(StringView locale, StringView language);
    
    /**
     * @brief Get the locale territory mapping object
     * 
     * @param locale 
     * @param territory 
     * @return Optional<StringView> 
     */
    Optional<StringView> get_locale_territory_mapping(StringView locale, StringView territory);

    /**
     * @brief Get the locale script mapping object
     * 
     * @param locale 
     * @param script 
     * @return Optional<StringView> 
     */
    Optional<StringView> get_locale_script_mapping(StringView locale, StringView script);

    /**
     * @brief Get the locale long currency mapping object
     * 
     * @param locale 
     * @param currency 
     * @return Optional<StringView> 
     */
    Optional<StringView> get_locale_long_currency_mapping(StringView locale, StringView currency);

    /**
     * @brief Get the locale short currency mapping object
     * 
     * @param locale 
     * @param currency 
     * @return Optional<StringView> 
     */
    Optional<StringView> get_locale_short_currency_mapping(StringView locale, StringView currency);

    /**
     * @brief Get the locale narrow currency mapping object
     * 
     * @param locale 
     * @param currency 
     * @return Optional<StringView> 
     */
    Optional<StringView> get_locale_narrow_currency_mapping(StringView locale, StringView currency);

    /**
     * @brief Get the locale numeric currency mapping object
     * 
     * @param locale 
     * @param currency 
     * @return Optional<StringView> 
     */
    Optional<StringView> get_locale_numeric_currency_mapping(StringView locale, StringView currency);

    /**
     * @brief Get the locale calendar mapping object
     * 
     * @param locale 
     * @param calendar 
     * @return Optional<StringView> 
     */
    Optional<StringView> get_locale_calendar_mapping(StringView locale, StringView calendar);

    /**
     * @brief Get the locale long date field mapping object
     * 
     * @param locale 
     * @param date_field 
     * @return Optional<StringView> 
     */
    Optional<StringView> get_locale_long_date_field_mapping(StringView locale, StringView date_field);

    /**
     * @brief Get the locale short date field mapping object
     * 
     * @param locale 
     * @param date_field 
     * @return Optional<StringView> 
     */
    Optional<StringView> get_locale_short_date_field_mapping(StringView locale, StringView date_field);

    /**
     * @brief Get the locale narrow date field mapping object
     * 
     * @param locale 
     * @param date_field 
     * @return Optional<StringView> 
     */
    Optional<StringView> get_locale_narrow_date_field_mapping(StringView locale, StringView date_field);

    /**
     * @brief Get the locale list patterns object
     * 
     * @param locale 
     * @param type 
     * @param style 
     * @return Optional<ListPatterns> 
     */
    Optional<ListPatterns> get_locale_list_patterns(StringView locale, StringView type, Style style);

    /**
     * @param language 
     * @return Optional<StringView> 
     */
    Optional<StringView> resolve_language_alias(StringView language);

    /**
     * @param territory 
     * @return Optional<StringView> 
     */
    Optional<StringView> resolve_territory_alias(StringView territory);

    /**
     * @param script_tag 
     * @return Optional<StringView> 
     */
    Optional<StringView> resolve_script_tag_alias(StringView script_tag);

    /**
     * @param variant 
     * @return Optional<StringView> 
     */
    Optional<StringView> resolve_variant_alias(StringView variant);

    /**
     * @param subdivision 
     * @return Optional<StringView> 
     */
    Optional<StringView> resolve_subdivision_alias(StringView subdivision);

    /**
     * @param language_id 
     */
    void resolve_complex_language_aliases(LanguageID& language_id);

    /**
     * @param language_id 
     * @return Optional<LanguageID> 
     */
    Optional<LanguageID> add_likely_subtags(LanguageID const& language_id); 

    /**
     * @param language_id 
     * @return Optional<LanguageID> 
     */
    Optional<LanguageID> remove_likely_subtags(LanguageID const& language_id);

    /**
     * @param language_id 
     * @return Optional<String> 
     */
    Optional<String> resolve_most_likely_territory(Unicode::LanguageID const& language_id);

    /**
     * @param language_id 
     * @param territory_alias 
     * @return String 
     */
    String resolve_most_likely_territory_alias(LanguageID const& language_id, StringView territory_alias);
} // namespace Unicode