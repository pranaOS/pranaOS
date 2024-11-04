/**
 * @file regexparser.h
 * @author Krisna Pranav
 * @brief Regex Parser
 * @version 6.0
 * @date 2024-11-04
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include "regexoptions.h"
#include <mods/forward.h>
#include <mods/string_builder.h>
#include <mods/types.h>
#include <mods/vector.h>
#include <libunicode/forward.h>
#include "regexbytecode.h"
#include "regexerror.h"
#include "regexlexer.h"

namespace regex 
{
    class PosixExtendedParser;
    class PosixBasicParser;
    class ECMA262Parser;

    /**
     * @tparam T 
     */
    template<typename T>
    struct GenericParserTraits 
    {
        using OptionsType = T;
    };

    /**
     * @tparam T 
     */
    template<typename T>
    struct ParserTraits : public GenericParserTraits<T> { };

    template<>
    struct ParserTraits<PosixExtendedParser> : public GenericParserTraits<PosixOptions> { };

    template<>
    struct ParserTraits<PosixBasicParser> : public GenericParserTraits<PosixOptions> { };

    template<>
    struct ParserTraits<ECMA262Parser> : public GenericParserTraits<ECMAScriptOptions> { };

    class Parser 
    {
    public:
        struct Result 
        {
            ByteCode bytecode;
            size_t capture_groups_count;
            size_t named_capture_groups_count;
            size_t match_length_minimum;
            Error error;
            Token error_token;
            Vector<FlyString> capture_groups;
            AllOptions options;
        }; // struct Result 

        /**
         * @brief Construct a new Parser object
         * 
         * @param lexer 
         */
        explicit Parser(Lexer& lexer)
            : m_parser_state(lexer)
        {
        }

        /**
         * @brief Construct a new Parser object
         * 
         * @param lexer 
         * @param regex_options 
         */
        Parser(Lexer& lexer, AllOptions regex_options)
            : m_parser_state(lexer, regex_options)
        {
        }

        /**
         * @brief Destroy the Parser object
         * 
         */
        virtual ~Parser() = default;

        /**
         * @param regex_options 
         * @return Result 
         */
        Result parse(Optional<AllOptions> regex_options = {});

        bool has_error() const 
        { 
            return m_parser_state.error != Error::NoError; 
        }

        Error error() const 
        { 
            return m_parser_state.error; 
        }

        AllOptions options() const 
        { 
            return m_parser_state.regex_options; 
        }

    protected:
        /**
         * @param match_length_minimum 
         * @return true 
         * @return false 
         */
        virtual bool parse_internal(ByteCode&, size_t& match_length_minimum) = 0;

        /**
         * @param type 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE bool match(TokenType type) const;

        /**
         * @param ch 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE bool match(char ch) const;
        
        ALWAYS_INLINE bool match_ordinary_characters();

        ALWAYS_INLINE Token consume();

        /**
         * @param type 
         * @param error 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE Token consume(TokenType type, Error error);
        
        ALWAYS_INLINE bool consume(String const&);
        
        /**
         * @param unicode 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE Optional<u32> consume_escaped_code_point(bool unicode);

        ALWAYS_INLINE bool try_skip(StringView);

        ALWAYS_INLINE bool lookahead_any(StringView);

        ALWAYS_INLINE unsigned char skip();

        ALWAYS_INLINE void back(size_t = 1);

        ALWAYS_INLINE void reset();

        ALWAYS_INLINE bool done() const;

        /**
         * @brief Set the error object
         * 
         * @param error 
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE bool set_error(Error error);

        struct NamedCaptureGroup 
        {
            size_t group_index { 0 };
            size_t minimum_length { 0 };
        }; // struct NamedCaptureGroup 

        struct ParserState 
        {
            Lexer& lexer;
            Token current_token;
            Error error = Error::NoError;
            Token error_token { TokenType::Eof, 0, StringView(nullptr) };
            ByteCode bytecode;
            size_t capture_groups_count { 0 };
            size_t named_capture_groups_count { 0 };
            size_t match_length_minimum { 0 };
            size_t repetition_mark_count { 0 };
            AllOptions regex_options;
            HashMap<int, size_t> capture_group_minimum_lengths;
            HashMap<FlyString, NamedCaptureGroup> named_capture_groups;

            /**
             * @brief Construct a new Parser State object
             * 
             * @param lexer 
             */
            explicit ParserState(Lexer& lexer)
                : lexer(lexer)
                , current_token(lexer.next())
            {
            }

            /**
             * @brief Construct a new Parser State object
             * 
             * @param lexer 
             * @param regex_options 
             */
            explicit ParserState(Lexer& lexer, AllOptions regex_options)
                : lexer(lexer)
                , current_token(lexer.next())
                , regex_options(regex_options)
            {
            }
        }; // struct ParserState 

        ParserState m_parser_state;
    }; // class Parser

    class AbstractPosixParser : public Parser 
    {
    protected:
        /**
         * @brief Construct a new Abstract Posix Parser object
         * 
         * @param lexer 
         */
        explicit AbstractPosixParser(Lexer& lexer)
            : Parser(lexer)
        {
        }

        /**
         * @brief Construct a new Abstract Posix Parser object
         * 
         * @param lexer 
         * @param regex_options 
         */
        AbstractPosixParser(Lexer& lexer, Optional<typename ParserTraits<PosixExtendedParser>::OptionsType> regex_options)
            : Parser(lexer, regex_options.value_or({}))
        {
        }

        /**
         * @return ALWAYS_INLINE 
         */
        ALWAYS_INLINE bool parse_bracket_expression(Vector<CompareTypeAndValuePair>&, size_t&);
    }; // class AbstractPosixParser : public Parser 

    class PosixBasicParser final : public AbstractPosixParser 
    {
    public:
        /**
         * @brief Construct a new Posix Basic Parser object
         * 
         * @param lexer 
         */
        explicit PosixBasicParser(Lexer& lexer)
            : AbstractPosixParser(lexer)
        {
        }

        /**
         * @brief Construct a new Posix Basic Parser object
         * 
         * @param lexer 
         * @param regex_options 
         */
        PosixBasicParser(Lexer& lexer, Optional<typename ParserTraits<PosixBasicParser>::OptionsType> regex_options)
            : AbstractPosixParser(lexer, regex_options.value_or({}))
        {
        }

        /**
         * @brief Destroy the Posix Basic Parser object
         * 
         */
        ~PosixBasicParser() = default;

    private:
        bool parse_internal(ByteCode&, size_t&) override;

        bool parse_root(ByteCode&, size_t&);
        bool parse_re_expression(ByteCode&, size_t&);
        bool parse_simple_re(ByteCode&, size_t&);
        bool parse_nonduplicating_re(ByteCode&, size_t&);
        bool parse_one_char_or_collation_element(ByteCode&, size_t&);

        constexpr static size_t number_of_addressable_capture_groups = 9;
        size_t m_capture_group_minimum_lengths[number_of_addressable_capture_groups] { 0 };
        bool m_capture_group_seen[number_of_addressable_capture_groups] { false };
        size_t m_current_capture_group_depth { 0 };
    }; // class PosixBasicParser final : public AbstractPosixParser 

    class PosixExtendedParser final : public AbstractPosixParser 
    {
        constexpr static auto default_options = static_cast<PosixFlags>(AllFlags::SingleLine) | static_cast<PosixFlags>(AllFlags::Internal_ConsiderNewline);

    public:
        /**
         * @brief Construct a new Posix Extended Parser object
         * 
         * @param lexer 
         */
        explicit PosixExtendedParser(Lexer& lexer)
            : AbstractPosixParser(lexer, default_options)
        {
        }

        /**
         * @brief Construct a new Posix Extended Parser object
         * 
         * @param lexer 
         * @param regex_options 
         */
        PosixExtendedParser(Lexer& lexer, Optional<typename ParserTraits<PosixExtendedParser>::OptionsType> regex_options)
            : AbstractPosixParser(lexer, regex_options.value_or({}) | default_options.value())
        {
        }

        /**
         * @brief Destroy the Posix Extended Parser object
         * 
         */
        ~PosixExtendedParser() = default;

    private:
        ALWAYS_INLINE bool match_repetition_symbol();

        bool parse_internal(ByteCode&, size_t&) override;

        bool parse_root(ByteCode&, size_t&);

        ALWAYS_INLINE bool parse_sub_expression(ByteCode&, size_t&);
        ALWAYS_INLINE bool parse_bracket_expression(ByteCode&, size_t&);
        ALWAYS_INLINE bool parse_repetition_symbol(ByteCode&, size_t&);
    }; // class PosixExtendedParser final : public AbstractPosixParser 

    class ECMA262Parser final : public Parser 
    {
        constexpr static ECMAScriptOptions default_options = static_cast<ECMAScriptFlags>(AllFlags::Internal_ConsiderNewline);

    public:
        /**
         * @brief Construct a new ECMA262Parser object
         * 
         * @param lexer 
         */
        explicit ECMA262Parser(Lexer& lexer)
            : Parser(lexer, default_options)
        {
            m_capture_groups_in_scope.empend();
        }   

        /**
         * @brief Construct a new ECMA262Parser object
         * 
         * @param lexer 
         * @param regex_options 
         */
        ECMA262Parser(Lexer& lexer, Optional<typename ParserTraits<ECMA262Parser>::OptionsType> regex_options)
            : Parser(lexer, regex_options.value_or({}) | default_options.value())
        {
            m_should_use_browser_extended_grammar = regex_options.has_value() && regex_options->has_flag_set(ECMAScriptFlags::BrowserExtended);
            m_capture_groups_in_scope.empend();
        }

        /**
         * @brief Destroy the ECMA262Parser object
         * 
         */
        ~ECMA262Parser() = default;

    private:
        bool parse_internal(ByteCode&, size_t&) override;

        enum class ReadDigitsInitialZeroState 
        {
            Allow,
            Disallow,
        }; // enum class ReadDigitsInitialZeroState 

        /**
         * @param initial_zero 
         * @param hex 
         * @param max_count 
         * @param min_count 
         * @return StringView 
         */
        StringView read_digits_as_string(ReadDigitsInitialZeroState initial_zero = ReadDigitsInitialZeroState::Allow, bool hex = false, int max_count = -1, int min_count = -1);

        /**
         * @param initial_zero 
         * @param hex 
         * @param max_count 
         * @param min_count 
         * @return Optional<unsigned> 
         */
        Optional<unsigned> read_digits(ReadDigitsInitialZeroState initial_zero = ReadDigitsInitialZeroState::Allow, bool hex = false, int max_count = -1, int min_count = -1);

        /**
         * @param take_starting_angle_bracket 
         * @return FlyString 
         */
        FlyString read_capture_group_specifier(bool take_starting_angle_bracket = false);

        struct Script 
        {
            Unicode::Script script {};
            bool is_extension { false };
        }; // struct Script 


        using PropertyEscape = Variant<Unicode::Property, Unicode::GeneralCategory, Script, Empty>;

        Optional<PropertyEscape> read_unicode_property_escape();

        /**
         * @param unicode 
         * @param named 
         * @return true 
         * @return false 
         */
        bool parse_pattern(ByteCode&, size_t&, bool unicode, bool named);

        /**
         * @param unicode 
         * @param named 
         * @return true 
         * @return false 
         */
        bool parse_disjunction(ByteCode&, size_t&, bool unicode, bool named);

        /**
         * @param unicode 
         * @param named 
         * @return true 
         * @return false 
         */
        bool parse_alternative(ByteCode&, size_t&, bool unicode, bool named);

        /**
         * @param unicode 
         * @param named 
         * @return true 
         * @return false 
         */
        bool parse_term(ByteCode&, size_t&, bool unicode, bool named);

        /**
         * @param unicode 
         * @param named 
         * @return true 
         * @return false 
         */
        bool parse_assertion(ByteCode&, size_t&, bool unicode, bool named);

        /**
         * @param unicode 
         * @param named 
         * @return true 
         * @return false 
         */
        bool parse_atom(ByteCode&, size_t&, bool unicode, bool named);

        /**
         * @param unicode 
         * @param named 
         * @return true 
         * @return false 
         */
        bool parse_quantifier(ByteCode&, size_t&, bool unicode, bool named);

        /**
         * @param repeat_min 
         * @param repeat_max 
         * @return true 
         * @return false 
         */
        bool parse_interval_quantifier(Optional<u64>& repeat_min, Optional<u64>& repeat_max);

        /**
         * @param unicode 
         * @param named 
         * @return true 
         * @return false 
         */
        bool parse_atom_escape(ByteCode&, size_t&, bool unicode, bool named);

        /**
         * @param unicode 
         * @param named 
         * @return true 
         * @return false 
         */
        bool parse_character_class(ByteCode&, size_t&, bool unicode, bool named);

        /**
         * @param unicode 
         * @param named 
         * @return true 
         * @return false 
         */
        bool parse_capture_group(ByteCode&, size_t&, bool unicode, bool named);

        /**
         * @param out_inverse 
         * @param expect_backslash 
         * @return Optional<CharClass> 
         */
        Optional<CharClass> parse_character_class_escape(bool& out_inverse, bool expect_backslash = false);

        /**
         * @param unicode 
         * @return true 
         * @return false 
         */
        bool parse_nonempty_class_ranges(Vector<CompareTypeAndValuePair>&, bool unicode);

        /**
         * @param property 
         * @param negated 
         * @return true 
         * @return false 
         */
        bool parse_unicode_property_escape(PropertyEscape& property, bool& negated);

        /**
         * @param named 
         * @return true 
         * @return false 
         */
        bool parse_quantifiable_assertion(ByteCode&, size_t&, bool named);

        /**
         * @param named 
         * @return true 
         * @return false 
         */
        bool parse_extended_atom(ByteCode&, size_t&, bool named);

        /**
         * @param bytecode_stack 
         * @param length 
         * @param unicode 
         * @param named 
         * @return true 
         * @return false 
         */
        bool parse_inner_disjunction(ByteCode& bytecode_stack, size_t& length, bool unicode, bool named);
        
        bool parse_invalid_braced_quantifier(); 
        
        /**
         * @param bytecode_stack 
         * @param length 
         * @return true 
         * @return false 
         */
        bool parse_legacy_octal_escape_sequence(ByteCode& bytecode_stack, size_t& length);

        Optional<u8> parse_legacy_octal_escape();

        size_t ensure_total_number_of_capturing_parenthesis();

        void enter_capture_group_scope() 
        { 
            m_capture_groups_in_scope.empend(); 
        }

        void exit_capture_group_scope()
        {
            auto last = m_capture_groups_in_scope.take_last();
            m_capture_groups_in_scope.last().extend(move(last));
        }

        /**
         * @param stack 
         */
        void clear_all_capture_groups_in_scope(ByteCode& stack)
        {
            for (auto& index : m_capture_groups_in_scope.last())
                stack.insert_bytecode_clear_capture_group(index);
        };

        Optional<size_t> m_total_number_of_capturing_parenthesis;

        bool m_should_use_browser_extended_grammar { false };

        Vector<Vector<size_t>> m_capture_groups_in_scope;
    }; // class ECMA262Parser final : public Parser

    using PosixExtended = PosixExtendedParser;
    using PosixBasic = PosixBasicParser;
    using ECMA262 = ECMA262Parser;
} // namespace regex

using regex::ECMA262;
using regex::PosixBasic;
using regex::PosixExtended;
