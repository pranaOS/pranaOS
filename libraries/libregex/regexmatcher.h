/**
 * @file regexmatcher.h
 * @author Krisna Pranav
 * @brief Regex Matcher
 * @version 6.0
 * @date 2024-11-04
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <ctype.h>
#include <stdio.h>
#include <mods/forward.h>
#include <mods/genericlexer.h>
#include <mods/hashmap.h>
#include <mods/nonnullownptrvector.h>
#include <mods/types.h>
#include <mods/utf32view.h>
#include <mods/vector.h>
#include "regexbytecode.h"
#include "regexmatch.h"
#include "regexoptions.h"
#include "regexparser.h"


namespace regex 
{

    namespace Detail 
    {
        struct Block 
        {
            size_t start;
            size_t end;
        }; // struct Block
    } // namespace Detail

    static constexpr const size_t c_max_recursion = 5000;
    static constexpr const size_t c_match_preallocation_count = 0;

    struct RegexResult final 
    {
        bool success { false };
        size_t count { 0 };
        Vector<Match> matches;
        Vector<Vector<Match>> capture_group_matches;
        size_t n_operations { 0 };
        size_t n_capture_groups { 0 };
        size_t n_named_capture_groups { 0 };
    }; // struct RegexResult final 

    /**
     * @tparam Parser 
     */
    template<class Parser>
    class Regex;

    /**
     * @tparam Parser 
     */
    template<class Parser>
    class Matcher final 
    {
    public:
        /**
         * @brief Construct a new Matcher object
         * 
         * @param pattern 
         * @param regex_options 
         */
        Matcher(Regex<Parser> const* pattern, Optional<typename ParserTraits<Parser>::OptionsType> regex_options = {})
            : m_pattern(pattern)
            , m_regex_options(regex_options.value_or({}))
        {
        }

        /**
         * @brief Destroy the Matcher object
         * 
         */
        ~Matcher() = default;

        /**
         * @return RegexResult 
         */
        RegexResult match(RegexStringView, Optional<typename ParserTraits<Parser>::OptionsType> = {}) const;
        
        RegexResult match(Vector<RegexStringView> const&, Optional<typename ParserTraits<Parser>::OptionsType> = {}) const;

        /**
         * @return ParserTraits<Parser>::OptionsType 
         */
        typename ParserTraits<Parser>::OptionsType options() const
        {
            return m_regex_options;
        }

        /**
         * @param pattern 
         */
        void reset_pattern(Badge<Regex<Parser>>, Regex<Parser> const* pattern)
        {
            m_pattern = pattern;
        }

    private:
        /**
         * @param input 
         * @param state 
         * @param operations 
         * @return true 
         * @return false 
         */
        bool execute(MatchInput const& input, MatchState& state, size_t& operations) const;

        Regex<Parser> const* m_pattern;

        typename ParserTraits<Parser>::OptionsType const m_regex_options;
    }; // class Matcher final 

    /**
     * @tparam Parser 
     */
    template<class Parser>
    class Regex final 
    {
    public:
        String pattern_value;
        regex::Parser::Result parser_result;
        OwnPtr<Matcher<Parser>> matcher { nullptr };
        mutable size_t start_offset { 0 };

        /**
         * @param pattern 
         * @param regex_options 
         * @return regex::Parser::Result 
         */
        static regex::Parser::Result parse_pattern(StringView pattern, typename ParserTraits<Parser>::OptionsType regex_options = {});

        /**
         * @brief Construct a new Regex object
         * 
         * @param pattern 
         * @param regex_options 
         */
        explicit Regex(String pattern, typename ParserTraits<Parser>::OptionsType regex_options = {});

        /**
         * @brief Construct a new Regex object
         * 
         * @param parse_result 
         * @param pattern 
         * @param regex_options 
         */
        Regex(regex::Parser::Result parse_result, String pattern, typename ParserTraits<Parser>::OptionsType regex_options = {});

        /**
         * @brief Destroy the Regex object
         * 
         */
        ~Regex() = default;

        /**
         * @brief Construct a new Regex object
         * 
         */
        Regex(Regex&&);

        /**
         * @return Regex& 
         */
        Regex& operator=(Regex&&);

        typename ParserTraits<Parser>::OptionsType options() const;

        /**
         * @param f 
         */
        void print_bytecode(FILE* f = stdout) const;

        /**
         * @param message 
         * @return String 
         */
        String error_string(Optional<String> message = {}) const;

        /**
         * @param view 
         * @param regex_options 
         * @return RegexResult 
         */
        RegexResult match(RegexStringView view, Optional<typename ParserTraits<Parser>::OptionsType> regex_options = {}) const
        {
            if (!matcher || parser_result.error != Error::NoError)
                return {};
            return matcher->match(view, regex_options);
        }

        /**
         * @param views 
         * @param regex_options 
         * @return RegexResult 
         */
        RegexResult match(Vector<RegexStringView> const& views, Optional<typename ParserTraits<Parser>::OptionsType> regex_options = {}) const
        {
            if (!matcher || parser_result.error != Error::NoError)
                return {};
            return matcher->match(views, regex_options);
        }

        /**
         * @param view 
         * @param replacement_pattern 
         * @param regex_options 
         * @return String 
         */
        String replace(RegexStringView view, StringView replacement_pattern, Optional<typename ParserTraits<Parser>::OptionsType> regex_options = {}) const
        {
            if (!matcher || parser_result.error != Error::NoError)
                return {};

            StringBuilder builder;
            size_t start_offset = 0;
            RegexResult result = matcher->match(view, regex_options);

            if (!result.success)
                return view.to_string();

            for (size_t i = 0; i < result.matches.size(); ++i) {
                auto& match = result.matches[i];
                builder.append(view.substring_view(start_offset, match.global_offset - start_offset).to_string());
                start_offset = match.global_offset + match.view.length();
                GenericLexer lexer(replacement_pattern);
                while (!lexer.is_eof()) {
                    if (lexer.consume_specific('\\')) {
                        if (lexer.consume_specific('\\')) {
                            builder.append('\\');
                            continue;
                        }
                        auto number = lexer.consume_while(isdigit);
                        if (auto index = number.to_uint(); index.has_value() && result.n_capture_groups >= index.value()) {
                            builder.append(result.capture_group_matches[i][index.value() - 1].view.to_string());
                        } else {
                            builder.appendff("\\{}", number);
                        }
                    } else {
                        builder.append(lexer.consume_while([](auto ch) { return ch != '\\'; }));
                    }
                }
            }

            builder.append(view.substring_view(start_offset, view.length() - start_offset).to_string());

            return builder.to_string();
        }

        /**
         * @param view 
         * @param regex_options 
         * @return RegexResult 
         */
        RegexResult search(RegexStringView view, Optional<typename ParserTraits<Parser>::OptionsType> regex_options = {}) const
        {
            if (!matcher || parser_result.error != Error::NoError)
                return {};

            AllOptions options = (AllOptions)regex_options.value_or({});
            if ((options & AllFlags::MatchNotBeginOfLine) && (options & AllFlags::MatchNotEndOfLine)) {
                options.reset_flag(AllFlags::MatchNotEndOfLine);
                options.reset_flag(AllFlags::MatchNotBeginOfLine);
            }
            options.reset_flag(AllFlags::Internal_Stateful);
            options |= AllFlags::Global;

            return matcher->match(view, options);
        }

        /**
         * @param views 
         * @param regex_options 
         * @return RegexResult 
         */
        RegexResult search(Vector<RegexStringView> const& views, Optional<typename ParserTraits<Parser>::OptionsType> regex_options = {}) const
        {
            if (!matcher || parser_result.error != Error::NoError)
                return {};

            AllOptions options = (AllOptions)regex_options.value_or({});
            if ((options & AllFlags::MatchNotBeginOfLine) && (options & AllFlags::MatchNotEndOfLine)) {
                options.reset_flag(AllFlags::MatchNotEndOfLine);
                options.reset_flag(AllFlags::MatchNotBeginOfLine);
            }
            options.reset_flag(AllFlags::Internal_Stateful);
            options |= AllFlags::Global;

            return matcher->match(views, options);
        }

        /**
         * @param view 
         * @param m 
         * @param regex_options 
         * @return true 
         * @return false 
         */
        bool match(RegexStringView view, RegexResult& m, Optional<typename ParserTraits<Parser>::OptionsType> regex_options = {}) const
        {
            m = match(view, regex_options);
            return m.success;
        }

        /**
         * @param views 
         * @param m 
         * @param regex_options 
         * @return true 
         * @return false 
         */
        bool match(Vector<RegexStringView> const& views, RegexResult& m, Optional<typename ParserTraits<Parser>::OptionsType> regex_options = {}) const
        {
            m = match(views, regex_options);
            return m.success;
        }

        /**
         * @param view 
         * @param m 
         * @param regex_options 
         * @return true 
         * @return false 
         */
        bool search(RegexStringView view, RegexResult& m, Optional<typename ParserTraits<Parser>::OptionsType> regex_options = {}) const
        {
            m = search(view, regex_options);
            return m.success;
        }

        /**
         * @param views 
         * @param m 
         * @param regex_options 
         * @return true 
         * @return false 
         */
        bool search(Vector<RegexStringView> const& views, RegexResult& m, Optional<typename ParserTraits<Parser>::OptionsType> regex_options = {}) const
        {
            m = search(views, regex_options);
            return m.success;
        }

        /**
         * @param view 
         * @param regex_options 
         * @return true 
         * @return false 
         */
        bool has_match(RegexStringView view, Optional<typename ParserTraits<Parser>::OptionsType> regex_options = {}) const
        {
            if (!matcher || parser_result.error != Error::NoError)
                return false;
            RegexResult result = matcher->match(view, AllOptions { regex_options.value_or({}) } | AllFlags::SkipSubExprResults);
            return result.success;
        }

        /**
         * @param views 
         * @param regex_options 
         * @return true 
         * @return false 
         */
        bool has_match(Vector<RegexStringView> const& views, Optional<typename ParserTraits<Parser>::OptionsType> regex_options = {}) const
        {
            if (!matcher || parser_result.error != Error::NoError)
                return false;
            RegexResult result = matcher->match(views, AllOptions { regex_options.value_or({}) } | AllFlags::SkipSubExprResults);
            return result.success;
        }

        using BasicBlockList = Vector<Detail::Block>;
        static BasicBlockList split_basic_blocks(ByteCode const&);

    private:
        void run_optimization_passes();
        void attempt_rewrite_loops_as_atomic_groups(BasicBlockList const&);
    }; // class Regex final

    /**
     * @tparam Parser 
     * @param view 
     * @param pattern 
     * @param regex_options 
     * @return RegexResult 
     */
    template<class Parser>
    RegexResult match(RegexStringView view, Regex<Parser>& pattern, Optional<typename ParserTraits<Parser>::OptionsType> regex_options = {})
    {
        return pattern.match(view, regex_options);
    }

    /**
     * @tparam Parser 
     * @param view 
     * @param pattern 
     * @param regex_options 
     * @return RegexResult 
     */
    template<class Parser>
    RegexResult match(Vector<RegexStringView> const& view, Regex<Parser>& pattern, Optional<typename ParserTraits<Parser>::OptionsType> regex_options = {})
    {
        return pattern.match(view, regex_options);
    }

    /**
     * @tparam Parser 
     * @param view 
     * @param pattern 
     * @param regex_options 
     * @return true 
     * @return false 
     */
    template<class Parser>
    bool match(RegexStringView view, Regex<Parser>& pattern, RegexResult&, Optional<typename ParserTraits<Parser>::OptionsType> regex_options = {})
    {
        return pattern.match(view, regex_options);
    }

    /**
     * @tparam Parser 
     * @param view 
     * @param pattern 
     * @param regex_options 
     * @return true 
     * @return false 
     */
    template<class Parser>
    bool match(Vector<RegexStringView> const& view, Regex<Parser>& pattern, RegexResult&, Optional<typename ParserTraits<Parser>::OptionsType> regex_options = {})
    {
        return pattern.match(view, regex_options);
    }
    
    /**
     * @tparam Parser 
     * @param view 
     * @param pattern 
     * @param regex_options 
     * @return RegexResult 
     */
    template<class Parser>
    RegexResult search(RegexStringView view, Regex<Parser>& pattern, Optional<typename ParserTraits<Parser>::OptionsType> regex_options = {})
    {
        return pattern.search(view, regex_options);
    }

    /**
     * @tparam Parser 
     * @param views 
     * @param pattern 
     * @param regex_options 
     * @return RegexResult 
     */
    template<class Parser>
    RegexResult search(Vector<RegexStringView> const& views, Regex<Parser>& pattern, Optional<typename ParserTraits<Parser>::OptionsType> regex_options = {})
    {
        return pattern.search(views, regex_options);
    }

    /**
     * @tparam Parser 
     * @param view 
     * @param pattern 
     * @param regex_options 
     * @return true 
     * @return false 
     */
    template<class Parser>
    bool search(RegexStringView view, Regex<Parser>& pattern, RegexResult&, Optional<typename ParserTraits<Parser>::OptionsType> regex_options = {})
    {
        return pattern.search(view, regex_options);
    }

    /**
     * @tparam Parser 
     * @param views 
     * @param pattern 
     * @param regex_options 
     * @return true 
     * @return false 
     */
    template<class Parser>
    bool search(Vector<RegexStringView> const& views, Regex<Parser>& pattern, RegexResult&, Optional<typename ParserTraits<Parser>::OptionsType> regex_options = {})
    {
        return pattern.search(views, regex_options);
    }

    /**
     * @tparam Parser 
     * @param view 
     * @param pattern 
     * @param regex_options 
     * @return true 
     * @return false 
     */
    template<class Parser>
    bool has_match(RegexStringView view, Regex<Parser>& pattern, Optional<typename ParserTraits<Parser>::OptionsType> regex_options = {})
    {
        return pattern.has_match(view, regex_options);
    }

    /**
     * @tparam Parser 
     * @param views 
     * @param pattern 
     * @param regex_options 
     * @return true 
     * @return false 
     */
    template<class Parser>
    bool has_match(Vector<RegexStringView> const& views, Regex<Parser>& pattern, Optional<typename ParserTraits<Parser>::OptionsType> regex_options = {})
    {
        return pattern.has_match(views, regex_options);
    }
} // namespace regex

using regex::has_match;
using regex::match;
using regex::Regex;
using regex::RegexResult;
