/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libweb/css/parser/StyleDeclarationRule.h>
#include <libweb/css/parser/StyleRule.h>

namespace Web::CSS {

class DeclarationOrAtRule {
    friend class Parser;

public:
    explicit DeclarationOrAtRule(RefPtr<StyleRule> at);
    explicit DeclarationOrAtRule(StyleDeclarationRule declaration);
    ~DeclarationOrAtRule();

    enum class DeclarationType {
        At,
        Declaration,
    };

    bool is_at_rule() const { return m_type == DeclarationType::At; }
    bool is_declaration() const { return m_type == DeclarationType::Declaration; }

    StyleRule const& at_rule() const
    {
        VERIFY(is_at_rule());
        return *m_at;
    }

    StyleDeclarationRule const& declaration() const
    {
        VERIFY(is_declaration());
        return m_declaration;
    }

    String to_string() const;

private:
    DeclarationType m_type;
    RefPtr<StyleRule> m_at;
    StyleDeclarationRule m_declaration;
};

}
