/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/RefCounted.h>
#include <base/Vector.h>
#include <libweb/css/parser/StyleBlockRule.h>
#include <libweb/css/parser/StyleComponentValueRule.h>

namespace Web::CSS {

class StyleRule : public RefCounted<StyleRule> {
    friend class Parser;

public:
    enum class Type {
        At,
        Qualified,
    };

    StyleRule(Type);
    ~StyleRule();

    Vector<StyleComponentValueRule> const& prelude() const { return m_prelude; }
    StyleBlockRule const& block() const { return *m_block; }

    String to_string() const;

private:
    Type const m_type;
    String m_name; 
    Vector<StyleComponentValueRule> m_prelude;
    RefPtr<StyleBlockRule> m_block;
};

}
