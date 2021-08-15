/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/RefCounted.h>
#include <base/String.h>
#include <base/Vector.h>

namespace Web::CSS {

class StyleComponentValueRule;

class StyleFunctionRule : public RefCounted<StyleFunctionRule> {
    friend class Parser;

public:
    StyleFunctionRule(String name);
    ~StyleFunctionRule();

    String const& name() const { return m_name; }
    Vector<StyleComponentValueRule> const& values() const { return m_values; }

    String to_string() const;

private:
    String m_name;
    Vector<StyleComponentValueRule> m_values;
};
}
