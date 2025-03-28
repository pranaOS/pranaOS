/**
 * @file regexmatch.h
 * @author Krisna Pranav
 * @brief Regex Match
 * @version 6.0
 * @date 2024-11-04
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "forward.h"
#include "regexoptions.h"

#include <mods/fly_string.h>
#include <mods/hashmap.h>
#include <mods/memmem.h>
#include <mods/string.h>
#include <mods/string_builder.h>
#include <mods/string_view.h>
#include <mods/utf16view.h>
#include <mods/utf32view.h>
#include <mods/utf8view.h>
#include <mods/variant.h>
#include <mods/vector.h>

namespace regex 
{
    class RegexStringView 
    {
    public:
        /**
         * @brief Construct a new Regex String View object
         * 
         * @param chars 
         */
        RegexStringView(char const* chars)
            : m_view(StringView { chars })
        {
        }

        /**
         * @brief Construct a new Regex String View object
         * 
         * @param string 
         */
        RegexStringView(String const& string)
            : m_view(string.view())
        {
        }

        /**
         * @brief Construct a new Regex String View object
         * 
         * @param view 
         */
        RegexStringView(StringView const view)
            : m_view(view)
        {
        }

        /**
         * @brief Construct a new Regex String View object
         * 
         * @param view 
         */
        RegexStringView(Utf32View view)
            : m_view(view)
        {
        }

        /**
         * @brief Construct a new Regex String View object
         * 
         * @param view 
         */
        RegexStringView(Utf16View view)
            : m_view(view)
        {
        }

        /**
         * @brief Construct a new Regex String View object
         * 
         * @param view 
         */
        RegexStringView(Utf8View view)
            : m_view(view)
        {
        }

        explicit RegexStringView(String&&) = delete;

        StringView string_view() const
        {
            return m_view.get<StringView>();
        }

        Utf32View const& u32_view() const
        {
            return m_view.get<Utf32View>();
        }

        Utf16View const& u16_view() const
        {
            return m_view.get<Utf16View>();
        }

        Utf8View const& u8_view() const
        {
            return m_view.get<Utf8View>();
        }

        bool unicode() const 
        { 
            return m_unicode; 
        }

        /**
         * @brief Set the unicode object
         * 
         * @param unicode 
         */
        void set_unicode(bool unicode) 
        { 
            m_unicode = unicode; 
        }

        /**
         * @return true 
         * @return false 
         */
        bool is_empty() const
        {
            return m_view.visit([](auto& view) { return view.is_empty(); });
        }

        bool is_null() const
        {
            return m_view.visit([](auto& view) { return view.is_null(); });
        }

        size_t length() const
        {
            if (unicode()) {
                return m_view.visit(
                    [](Utf16View const& view) { return view.length_in_code_points(); },
                    [](auto const& view) { return view.length(); });
            }

            return length_in_code_units();
        }

        size_t length_in_code_units() const
        {
            return m_view.visit(
                [](Utf16View const& view) { return view.length_in_code_units(); },
                [](Utf8View const& view) { return view.byte_length(); },
                [](auto const& view) { return view.length(); });
        }

        /**
         * @param code_point 
         * @return size_t 
         */
        size_t length_of_code_point(u32 code_point) const
        {
            return m_view.visit(
                [](Utf32View const&) { return 1; },
                [&](Utf16View const&) {
                    if (code_point < 0x10000)
                        return 1;
                    return 2;
                },
                [&](auto const&) {
                    if (code_point <= 0x7f)
                        return 1;
                    if (code_point <= 0x07ff)
                        return 2;
                    if (code_point <= 0xffff)
                        return 3;
                    return 4;
                });
        }

        /**
         * @return RegexStringView 
         */
        RegexStringView typed_null_view()
        {
            auto view = m_view.visit(
                [&]<typename T>(T const&) {
                    return RegexStringView { T {} };
                });
            view.set_unicode(unicode());
            return view;
        }

        /**
         * @param data 
         * @param optional_string_storage 
         * @param optional_utf16_storage 
         * @return RegexStringView 
         */
        RegexStringView construct_as_same(Span<u32> data, Optional<String>& optional_string_storage, Vector<u16, 1>& optional_utf16_storage) const
        {
            auto view = m_view.visit(
                [&]<typename T>(T const&) {
                    StringBuilder builder;
                    for (auto ch : data)
                        builder.append(ch); 
                    optional_string_storage = builder.build();
                    return RegexStringView { T { *optional_string_storage } };
                },
                [&](Utf32View) {
                    return RegexStringView { Utf32View { data.data(), data.size() } };
                },
                [&](Utf16View) {
                    optional_utf16_storage = Mods::utf32_to_utf16(Utf32View { data.data(), data.size() });
                    return RegexStringView { Utf16View { optional_utf16_storage } };
                });

            view.set_unicode(unicode());
            return view;
        }

        /**
         * @return Vector<RegexStringView> 
         */
        Vector<RegexStringView> lines() const
        {
            return m_view.visit(
                [](StringView view) {
                    auto views = view.lines(false);
                    Vector<RegexStringView> new_views;
                    for (auto& view : views)
                        new_views.empend(view);
                    return new_views;
                },
                [](Utf32View view) {
                    if (view.is_empty())
                        return Vector<RegexStringView> { view };

                    Vector<RegexStringView> views;
                    u32 newline = '\n';
                    while (!view.is_empty()) {
                        auto position = Mods::memmem_optional(view.code_points(), view.length() * sizeof(u32), &newline, sizeof(u32));
                        if (!position.has_value())
                            break;
                        auto offset = position.value() / sizeof(u32);
                        views.empend(view.substring_view(0, offset));
                        view = view.substring_view(offset + 1, view.length() - offset - 1);
                    }
                    if (!view.is_empty())
                        views.empend(view);
                    return views;
                },
                [](Utf16View view) {
                    if (view.is_empty())
                        return Vector<RegexStringView> { view };

                    Vector<RegexStringView> views;
                    u16 newline = '\n';
                    while (!view.is_empty()) {
                        auto position = Mods::memmem_optional(view.data(), view.length_in_code_units() * sizeof(u16), &newline, sizeof(u16));
                        if (!position.has_value())
                            break;
                        auto offset = position.value() / sizeof(u16);
                        views.empend(view.substring_view(0, offset));
                        view = view.substring_view(offset + 1, view.length_in_code_units() - offset - 1);
                    }
                    if (!view.is_empty())
                        views.empend(view);
                    return views;
                },
                [](Utf8View const& view) {
                    if (view.is_empty())
                        return Vector<RegexStringView> { view };

                    Vector<RegexStringView> views;
                    auto it = view.begin();
                    auto previous_newline_position_it = it;
                    for (;;) {
                        if (*it == '\n') {
                            auto previous_offset = view.byte_offset_of(previous_newline_position_it);
                            auto new_offset = view.byte_offset_of(it);
                            auto slice = view.substring_view(previous_offset, new_offset - previous_offset);
                            views.empend(slice);
                            ++it;
                            previous_newline_position_it = it;
                        }
                        if (it.done())
                            break;
                        ++it;
                    }
                    if (it != previous_newline_position_it) {
                        auto previous_offset = view.byte_offset_of(previous_newline_position_it);
                        auto new_offset = view.byte_offset_of(it);
                        auto slice = view.substring_view(previous_offset, new_offset - previous_offset);
                        views.empend(slice);
                    }
                    return views;
                });
        }

        /**
         * @param offset 
         * @param length 
         * @return RegexStringView 
         */
        RegexStringView substring_view(size_t offset, size_t length) const
        {
            if (unicode()) {
                auto view = m_view.visit(
                    [&](auto view) { return RegexStringView { view.substring_view(offset, length) }; },
                    [&](Utf16View const& view) { return RegexStringView { view.unicode_substring_view(offset, length) }; },
                    [&](Utf8View const& view) { return RegexStringView { view.unicode_substring_view(offset, length) }; });

                view.set_unicode(unicode());
                return view;
            }

            auto view = m_view.visit([&](auto view) { return RegexStringView { view.substring_view(offset, length) }; });
            view.set_unicode(unicode());
            return view;
        }

        /**
         * @return String 
         */
        String to_string() const
        {
            return m_view.visit(
                [](StringView view) { return view.to_string(); },
                [](Utf16View view) { return view.to_utf8(Utf16View::AllowInvalidCodeUnits::Yes); },
                [](auto& view) {
                    StringBuilder builder;
                    for (auto it = view.begin(); it != view.end(); ++it)
                        builder.append_code_point(*it);
                    return builder.to_string();
                });
        }

        /**
         * @param index 
         * @return u32 
         */
        u32 operator[](size_t index) const
        {
            return m_view.visit(
                [&](StringView view) -> u32 {
                    auto ch = view[index];
                    if (ch < 0)
                        return 256u + ch;
                    return ch;
                },
                [&](Utf32View const& view) -> u32 { return view[index]; },
                [&](Utf16View const& view) -> u32 { return view.code_point_at(index); },
                [&](Utf8View const& view) -> u32 {
                    auto it = view.iterator_at_byte_offset(index);
                    VERIFY(it != view.end());
                    return *it;
                });
        }

        /**
         * @param code_point_index 
         * @return size_t 
         */
        size_t code_unit_offset_of(size_t code_point_index) const
        {
            return m_view.visit(
                [&](StringView view) -> u32 {
                    Utf8View utf8_view { view };
                    return utf8_view.byte_offset_of(code_point_index);
                },
                [&](Utf32View const&) -> u32 { return code_point_index; },
                [&](Utf16View const& view) -> u32 {
                    return view.code_unit_offset_of(code_point_index);
                },
                [&](Utf8View const& view) -> u32 {
                    return view.byte_offset_of(code_point_index);
                });
        }

        /**
         * @param cstring 
         * @return true 
         * @return false 
         */
        bool operator==(char const* cstring) const
        {
            return m_view.visit(
                [&](Utf32View) { return to_string() == cstring; },
                [&](Utf16View) { return to_string() == cstring; },
                [&](Utf8View const& view) { return view.as_string() == cstring; },
                [&](StringView view) { return view == cstring; });
        }

        /**
         * @param cstring 
         * @return true 
         * @return false 
         */
        bool operator!=(char const* cstring) const
        {
            return !(*this == cstring);
        }

        /**
         * @param string 
         * @return true 
         * @return false 
         */
        bool operator==(String const& string) const
        {
            return m_view.visit(
                [&](Utf32View) { return to_string() == string; },
                [&](Utf16View) { return to_string() == string; },
                [&](Utf8View const& view) { return view.as_string() == string; },
                [&](StringView view) { return view == string; });
        }

        /**
         * @param string 
         * @return true 
         * @return false 
         */
        bool operator==(StringView string) const
        {
            return m_view.visit(
                [&](Utf32View) { return to_string() == string; },
                [&](Utf16View) { return to_string() == string; },
                [&](Utf8View const& view) { return view.as_string() == string; },
                [&](StringView view) { return view == string; });
        }       

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(StringView other) const
        {
            return !(*this == other);
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(Utf32View const& other) const
        {
            return m_view.visit(
                [&](Utf32View view) {
                    return view.length() == other.length() && __builtin_memcmp(view.code_points(), other.code_points(), view.length() * sizeof(u32)) == 0;
                },
                [&](Utf16View) { return to_string() == RegexStringView { other }.to_string(); },
                [&](Utf8View const& view) { return view.as_string() == RegexStringView { other }.to_string(); },
                [&](StringView view) { return view == RegexStringView { other }.to_string(); });
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(Utf32View const& other) const
        {
            return !(*this == other);
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(Utf16View const& other) const
        {
            return m_view.visit(
                [&](Utf32View) { return to_string() == RegexStringView { other }.to_string(); },
                [&](Utf16View const& view) { return view == other; },
                [&](Utf8View const& view) { return view.as_string() == RegexStringView { other }.to_string(); },
                [&](StringView view) { return view == RegexStringView { other }.to_string(); });
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(Utf16View const& other) const
        {
            return !(*this == other);
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(Utf8View const& other) const
        {
            return m_view.visit(
                [&](Utf32View) { return to_string() == other.as_string(); },
                [&](Utf16View) { return to_string() == other.as_string(); },
                [&](Utf8View const& view) { return view.as_string() == other.as_string(); },
                [&](StringView view) { return other.as_string() == view; });
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(Utf8View const& other) const
        {
            return !(*this == other);
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool equals(RegexStringView other) const
        {
            return other.m_view.visit([this](auto const& view) { return operator==(view); });
        }

        /**
         * @param other 
         * @return true 
         * @return false 
         */
        bool equals_ignoring_case(RegexStringView other) const
        {
            return m_view.visit(
                [&](StringView view) {
                    return other.m_view.visit(
                        [&](StringView other_view) { return view.equals_ignoring_case(other_view); },
                        [](auto&) -> bool { TODO(); });
                },
                [&](Utf16View view) {
                    return other.m_view.visit(
                        [&](Utf16View other_view) { return view.equals_ignoring_case(other_view); },
                        [](auto&) -> bool { TODO(); });
                },
                [](auto&) -> bool { TODO(); });
        }

        /**
         * @param str 
         * @return true 
         * @return false 
         */
        bool starts_with(StringView str) const
        {
            return m_view.visit(
                [&](Utf32View) -> bool {
                    TODO();
                },
                [&](Utf16View) -> bool {
                    TODO();
                },
                [&](Utf8View const& view) { return view.as_string().starts_with(str); },
                [&](StringView view) { return view.starts_with(str); });
        }       

        /**
         * @param str 
         * @return true 
         * @return false 
         */
        bool starts_with(Utf32View const& str) const
        {
            return m_view.visit(
                [&](Utf32View view) -> bool {
                    if (str.length() > view.length())
                        return false;
                    if (str.length() == view.length())
                        return operator==(str);
                    for (size_t i = 0; i < str.length(); ++i) {
                        if (str.at(i) != view.at(i))
                            return false;
                    }
                    return true;
                },
                [&](Utf16View) -> bool { TODO(); },
                [&](Utf8View const& view) {
                    auto it = view.begin();
                    for (auto code_point : str) {
                        if (it.done())
                            return false;
                        if (code_point != *it)
                            return false;
                        ++it;
                    }
                    return true;
                },

                [&](StringView) -> bool { TODO(); });
        }

    private:
        Variant<StringView, Utf8View, Utf16View, Utf32View> m_view;
        bool m_unicode { false };
    };

    class Match final 
    {
    private:
        Optional<FlyString> string;

    public:
        /**
         * @brief Construct a new Match object
         * 
         */
        Match() = default;

        /**
         * @brief Destroy the Match object
         * 
         */
        ~Match() = default;

        /**
         * @brief Construct a new Match object
         * 
         * @param view_ 
         * @param line_ 
         * @param column_ 
         * @param global_offset_ 
         */
        Match(RegexStringView view_, size_t const line_, size_t const column_, size_t const global_offset_)
            : view(view_)
            , line(line_)
            , column(column_)
            , global_offset(global_offset_)
            , left_column(column_)
        {
        }

        /**
         * @brief Construct a new Match object
         * 
         * @param string_ 
         * @param line_ 
         * @param column_ 
         * @param global_offset_ 
         */
        Match(String string_, size_t const line_, size_t const column_, size_t const global_offset_)
            : string(move(string_))
            , view(string.value().view())
            , line(line_)
            , column(column_)
            , global_offset(global_offset_)
        {
        }

        /**
         * @param view_
         * @param capture_group_name_
         * @param line_
         * @param column_
         * @param global_offset_
         * @param column_
         * 
         */
        Match(RegexStringView const view_, StringView capture_group_name_, size_t const line_, size_t const column_, size_t const global_offset_)
            : view(view_)
            , capture_group_name(capture_group_name_)
            , line(line_)
            , column(column_)
            , global_offset(global_offset_)
            , left_column(column_)
        {
        }

        void reset()
        {
            view = view.typed_null_view();
            capture_group_name.clear();
            line = 0;
            column = 0;
            global_offset = 0;
            left_column = 0;
        }

        RegexStringView view { nullptr };
        Optional<FlyString> capture_group_name {};
        size_t line { 0 };
        size_t column { 0 };
        size_t global_offset { 0 };

        size_t left_column { 0 };
    }; // class Match final

    struct MatchInput 
    {
        RegexStringView view { nullptr };
        AllOptions regex_options {};
        size_t start_offset { 0 }; 

        size_t match_index { 0 };
        size_t line { 0 };
        size_t column { 0 };

        size_t global_offset { 0 }; 

        mutable size_t fail_counter { 0 };
        mutable Vector<size_t> saved_positions;
        mutable Vector<size_t> saved_code_unit_positions;
        mutable Vector<size_t> saved_forks_since_last_save;
        mutable HashMap<u64, u64> checkpoints;
        mutable Optional<size_t> fork_to_replace;
    }; // struct MatchInput 

    struct MatchState 
    {
        size_t string_position_before_match { 0 };
        size_t string_position { 0 };
        size_t string_position_in_code_units { 0 };
        size_t instruction_position { 0 };
        size_t fork_at_position { 0 };
        size_t forks_since_last_save { 0 };
        Optional<size_t> initiating_fork;
        Vector<Match> matches;
        Vector<Vector<Match>> capture_group_matches;
        Vector<u64> repetition_marks;
    }; // struct MatchState 
} // namespace regex

using regex::RegexStringView;

template<>
struct Mods::Formatter<regex::RegexStringView> : Formatter<StringView> 
{
    /**
     * @param builder 
     * @param value 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> format(FormatBuilder& builder, regex::RegexStringView value)
    {
        auto string = value.to_string();
        return Formatter<StringView>::format(builder, string);
    }
};
