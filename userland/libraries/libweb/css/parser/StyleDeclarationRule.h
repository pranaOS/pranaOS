/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/String.h>
#include <base/Vector.h>
#include <libweb/css/parser/StyleComponentValueRule.h>

namespace Web::CSS {

class StyleDeclarationRule {
    friend class Parser;

public:
    StyleDeclarationRule();
    ~StyleDeclarationRule();

    String to_string() const;

private:
    String m_name;
    Vector<StyleComponentValueRule> m_values;
    bool m_important { false };
};

}
