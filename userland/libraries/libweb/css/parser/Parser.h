/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/NonnullOwnPtrVector.h>
#include <base/NonnullRefPtrVector.h>
#include <base/RefPtr.h>
#include <base/Result.h>
#include <base/Vector.h>
#include <libweb/css/parser/DeclarationOrAtRule.h>
#include <libweb/css/parser/StyleBlockRule.h>
#include <libweb/css/parser/StyleComponentValueRule.h>
#include <libweb/css/parser/StyleDeclarationRule.h>
#include <libweb/css/parser/StyleFunctionRule.h>
#include <libweb/css/parser/StyleRule.h>
#include <libweb/css/parser/Tokenizer.h>
#include <libweb/css/Selector.h>
#include <libweb/css/StyleValue.h>

namespace Web::CSS {

class CSSStyleSheet;
class CSSRule;
class CSSStyleRule;
struct StyleProperty;
enum class PropertyID;

class ParsingContext {
public:
    ParsingContext();
    explicit ParsingContext(DOM::Document&);
    explicit ParsingContext(DOM::ParentNode&);

    bool in_quirks_mode() const;
    DOM::Document* document() const { return m_document; }
    URL complete_url(String const&) const;

private:
    DOM::Document* m_document { nullptr };
};

template<typename T>
class TokenStream {
public:
    explicit TokenStream(Vector<T> const&);
    ~TokenStream();

    bool has_next_token();
    T const& next_token();
    T const& peek_token(int offset = 0);
    T const& current_token();
    void reconsume_current_input_token();

    void skip_whitespace();

    void dump_all_tokens();

private:
    Vector<T> const& m_tokens;
    int m_iterator_offset { -1 };

    T make_eof();
    T m_eof;
};

class Parser {
public:
    Parser(ParsingContext const&, StringView const& input, String const& encoding = "utf-8");
    ~Parser();

    NonnullRefPtr<CSSStyleSheet> parse_as_stylesheet();

    NonnullRefPtrVector<CSSRule> parse_as_list_of_rules();

    RefPtr<CSSRule> parse_as_rule();

    Optional<StyleProperty> parse_as_declaration();
 
    RefPtr<CSSStyleDeclaration> parse_as_list_of_declarations();

    Optional<StyleComponentValueRule> parse_as_component_value();

    Vector<StyleComponentValueRule> parse_as_list_of_component_values();
    Vector<Vector<StyleComponentValueRule>> parse_as_comma_separated_list_of_component_values();

    Optional<SelectorList> parse_as_selector();
    Optional<SelectorList> parse_as_relative_selector();

    RefPtr<StyleValue> parse_as_css_value(PropertyID);

    RefPtr<StyleValue> parse_css_value(PropertyID, TokenStream<StyleComponentValueRule>&);
    static RefPtr<StyleValue> parse_css_value(ParsingContext const&, PropertyID, StyleComponentValueRule const&);

private:
    template<typename T>
    NonnullRefPtr<CSSStyleSheet> parse_a_stylesheet(TokenStream<T>&);
    template<typename T>
    NonnullRefPtrVector<CSSRule> parse_a_list_of_rules(TokenStream<T>&);
    template<typename T>
    RefPtr<CSSRule> parse_a_rule(TokenStream<T>&);
    template<typename T>
    Optional<StyleProperty> parse_a_declaration(TokenStream<T>&);
    template<typename T>
    RefPtr<CSSStyleDeclaration> parse_a_list_of_declarations(TokenStream<T>&);
    template<typename T>
    Optional<StyleComponentValueRule> parse_a_component_value(TokenStream<T>&);
    template<typename T>
    Vector<StyleComponentValueRule> parse_a_list_of_component_values(TokenStream<T>&);
    template<typename T>
    Vector<Vector<StyleComponentValueRule>> parse_a_comma_separated_list_of_component_values(TokenStream<T>&);
    template<typename T>
    Optional<SelectorList> parse_a_selector(TokenStream<T>&);
    template<typename T>
    Optional<SelectorList> parse_a_relative_selector(TokenStream<T>&);
    template<typename T>
    Optional<SelectorList> parse_a_selector_list(TokenStream<T>&);
    template<typename T>
    Optional<SelectorList> parse_a_relative_selector_list(TokenStream<T>&);

    Optional<Selector::SimpleSelector::ANPlusBPattern> parse_a_n_plus_b_pattern(TokenStream<StyleComponentValueRule>&);

    [[nodiscard]] NonnullRefPtrVector<StyleRule> consume_a_list_of_rules(bool top_level);
    template<typename T>
    [[nodiscard]] NonnullRefPtrVector<StyleRule> consume_a_list_of_rules(TokenStream<T>&, bool top_level);

    [[nodiscard]] NonnullRefPtr<StyleRule> consume_an_at_rule();
    template<typename T>
    [[nodiscard]] NonnullRefPtr<StyleRule> consume_an_at_rule(TokenStream<T>&);

    [[nodiscard]] RefPtr<StyleRule> consume_a_qualified_rule();
    template<typename T>
    [[nodiscard]] RefPtr<StyleRule> consume_a_qualified_rule(TokenStream<T>&);

    [[nodiscard]] Vector<DeclarationOrAtRule> consume_a_list_of_declarations();
    template<typename T>
    [[nodiscard]] Vector<DeclarationOrAtRule> consume_a_list_of_declarations(TokenStream<T>&);

    [[nodiscard]] Optional<StyleDeclarationRule> consume_a_declaration();
    template<typename T>
    [[nodiscard]] Optional<StyleDeclarationRule> consume_a_declaration(TokenStream<T>&);

    [[nodiscard]] StyleComponentValueRule consume_a_component_value();
    template<typename T>
    [[nodiscard]] StyleComponentValueRule consume_a_component_value(TokenStream<T>&);

    [[nodiscard]] NonnullRefPtr<StyleBlockRule> consume_a_simple_block();
    template<typename T>
    [[nodiscard]] NonnullRefPtr<StyleBlockRule> consume_a_simple_block(TokenStream<T>&);

    [[nodiscard]] NonnullRefPtr<StyleFunctionRule> consume_a_function();
    template<typename T>
    [[nodiscard]] NonnullRefPtr<StyleFunctionRule> consume_a_function(TokenStream<T>&);

    [[nodiscard]] RefPtr<CSSRule> convert_to_rule(NonnullRefPtr<StyleRule>);
    [[nodiscard]] RefPtr<CSSStyleDeclaration> convert_to_declaration(NonnullRefPtr<StyleBlockRule>);
    [[nodiscard]] Optional<StyleProperty> convert_to_style_property(StyleDeclarationRule&);

    static Optional<float> try_parse_float(StringView string);
    static Optional<Color> parse_color(ParsingContext const&, StyleComponentValueRule const&);
    static Optional<Length> parse_length(ParsingContext const&, StyleComponentValueRule const&);
    static Optional<URL> parse_url_function(ParsingContext const&, StyleComponentValueRule const&);

    static RefPtr<StyleValue> parse_builtin_or_dynamic_value(ParsingContext const&, StyleComponentValueRule const&);
    static RefPtr<StyleValue> parse_length_value(ParsingContext const&, StyleComponentValueRule const&);
    static RefPtr<StyleValue> parse_numeric_value(ParsingContext const&, StyleComponentValueRule const&);
    static RefPtr<StyleValue> parse_identifier_value(ParsingContext const&, StyleComponentValueRule const&);
    static RefPtr<StyleValue> parse_color_value(ParsingContext const&, StyleComponentValueRule const&);
    static RefPtr<StyleValue> parse_string_value(ParsingContext const&, StyleComponentValueRule const&);
    static RefPtr<StyleValue> parse_image_value(ParsingContext const&, StyleComponentValueRule const&);
    static RefPtr<StyleValue> parse_box_shadow_value(ParsingContext const&, Vector<StyleComponentValueRule> const&);

    static OwnPtr<CalculatedStyleValue::CalcSum> parse_calc_sum(ParsingContext const&, TokenStream<StyleComponentValueRule>&);
    static OwnPtr<CalculatedStyleValue::CalcProduct> parse_calc_product(ParsingContext const&, TokenStream<StyleComponentValueRule>&);
    static Optional<CalculatedStyleValue::CalcValue> parse_calc_value(ParsingContext const&, TokenStream<StyleComponentValueRule>&);
    static OwnPtr<CalculatedStyleValue::CalcNumberSum> parse_calc_number_sum(ParsingContext const&, TokenStream<StyleComponentValueRule>&);
    static OwnPtr<CalculatedStyleValue::CalcNumberProduct> parse_calc_number_product(ParsingContext const&, TokenStream<StyleComponentValueRule>&);
    static Optional<CalculatedStyleValue::CalcNumberValue> parse_calc_number_value(ParsingContext const&, TokenStream<StyleComponentValueRule>&);
    static OwnPtr<CalculatedStyleValue::CalcProductPartWithOperator> parse_calc_product_part_with_operator(ParsingContext const&, TokenStream<StyleComponentValueRule>&);
    static OwnPtr<CalculatedStyleValue::CalcSumPartWithOperator> parse_calc_sum_part_with_operator(ParsingContext const&, TokenStream<StyleComponentValueRule>&);
    static OwnPtr<CalculatedStyleValue::CalcNumberProductPartWithOperator> parse_calc_number_product_part_with_operator(ParsingContext const&, TokenStream<StyleComponentValueRule>& tokens);
    static OwnPtr<CalculatedStyleValue::CalcNumberSumPartWithOperator> parse_calc_number_sum_part_with_operator(ParsingContext const&, TokenStream<StyleComponentValueRule>&);
    static OwnPtr<CalculatedStyleValue::CalcSum> parse_calc_expression(ParsingContext const&, Vector<StyleComponentValueRule> const&);

    enum class SelectorParsingResult {
        Done,
        SyntaxError,
    };

    RefPtr<Selector> parse_complex_selector(TokenStream<StyleComponentValueRule>&, bool allow_starting_combinator);
    Result<Selector::CompoundSelector, SelectorParsingResult> parse_compound_selector(TokenStream<StyleComponentValueRule>&);
    Optional<Selector::Combinator> parse_selector_combinator(TokenStream<StyleComponentValueRule>&);
    Result<Selector::SimpleSelector, SelectorParsingResult> parse_simple_selector(TokenStream<StyleComponentValueRule>&);

    ParsingContext m_context;

    Tokenizer m_tokenizer;
    Vector<Token> m_tokens;
    TokenStream<Token> m_token_stream;
};

}

namespace Web {

RefPtr<CSS::CSSStyleSheet> parse_css(CSS::ParsingContext const&, StringView const&);
RefPtr<CSS::CSSStyleDeclaration> parse_css_declaration(CSS::ParsingContext const&, StringView const&);
RefPtr<CSS::StyleValue> parse_css_value(CSS::ParsingContext const&, StringView const&, CSS::PropertyID property_id = CSS::PropertyID::Invalid);
Optional<CSS::SelectorList> parse_selector(CSS::ParsingContext const&, StringView const&);

RefPtr<CSS::StyleValue> parse_html_length(DOM::Document const&, StringView const&);

}
