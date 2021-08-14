/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/Result.h>
#include <libjs/AST.h>
#include <libjs/runtime/Object.h>
#include <libregex/Regex.h>

namespace JS {

RegExpObject* regexp_create(GlobalObject&, Value pattern, Value flags);

Result<regex::RegexOptions<ECMAScriptFlags>, String> regex_flags_from_string(StringView flags);
String parse_regex_pattern(StringView pattern, bool unicode);

class RegExpObject : public Object {
    JS_OBJECT(RegExpObject, Object);

public:

    static constexpr regex::RegexOptions<ECMAScriptFlags> default_flags { (regex::ECMAScriptFlags)regex::AllFlags::Global | (regex::ECMAScriptFlags)regex::AllFlags::SkipTrimEmptyMatches | regex::ECMAScriptFlags::BrowserExtended };

    static RegExpObject* create(GlobalObject&, Regex<ECMA262> regex, String pattern, String flags);

    RegExpObject(Regex<ECMA262> regex, String pattern, String flags, Object& prototype);
    virtual void initialize(GlobalObject&) override;
    virtual ~RegExpObject() override;

    const String& pattern() const { return m_pattern; }
    const String& flags() const { return m_flags; }
    const Regex<ECMA262>& regex() { return m_regex; }
    const Regex<ECMA262>& regex() const { return m_regex; }

private:
    String m_pattern;
    String m_flags;
    Regex<ECMA262> m_regex;
};

}
