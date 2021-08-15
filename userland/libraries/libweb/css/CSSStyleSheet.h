/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/NonnullRefPtrVector.h>
#include <base/TypeCasts.h>
#include <libweb/css/CSSImportRule.h>
#include <libweb/css/CSSRule.h>
#include <libweb/css/StyleSheet.h>
#include <libweb/loader/Resource.h>

namespace Web::CSS {

class CSSStyleSheet final : public StyleSheet {
public:
    using WrapperType = Bindings::CSSStyleSheetWrapper;

    static NonnullRefPtr<CSSStyleSheet> create(NonnullRefPtrVector<CSSRule> rules)
    {
        return adopt_ref(*new CSSStyleSheet(move(rules)));
    }

    virtual ~CSSStyleSheet() override;

    virtual String type() const override { return "text/css"; }

    const NonnullRefPtrVector<CSSRule>& rules() const { return m_rules; }
    NonnullRefPtrVector<CSSRule>& rules() { return m_rules; }

    template<typename Callback>
    void for_each_effective_style_rule(Callback callback) const
    {
        for (auto& rule : m_rules)
            if (rule.type() == CSSRule::Type::Style) {
                callback(verify_cast<CSSStyleRule>(rule));
            } else if (rule.type() == CSSRule::Type::Import) {
                const auto& import_rule = verify_cast<CSSImportRule>(rule);
                if (import_rule.has_import_result())
                    import_rule.loaded_style_sheet()->for_each_effective_style_rule(callback);
            }
    }

    template<typename Callback>
    bool for_first_not_loaded_import_rule(Callback callback)
    {
        for (auto& rule : m_rules)
            if (rule.type() == CSSRule::Type::Import) {
                auto& import_rule = verify_cast<CSSImportRule>(rule);
                if (!import_rule.has_import_result()) {
                    callback(import_rule);
                    return true;
                }

                if (import_rule.loaded_style_sheet()->for_first_not_loaded_import_rule(callback)) {
                    return true;
                }
            }

        return false;
    }

private:
    explicit CSSStyleSheet(NonnullRefPtrVector<CSSRule>);

    NonnullRefPtrVector<CSSRule> m_rules;
};

}

namespace Web::Bindings {

CSSStyleSheetWrapper* wrap(JS::GlobalObject&, CSS::CSSStyleSheet&);

}
