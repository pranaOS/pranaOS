/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/RefCounted.h>
#include <base/String.h>
#include <libweb/css/CSSStyleDeclaration.h>
#include <libweb/css/Selector.h>

namespace Web::CSS {

class CSSRule : public RefCounted<CSSRule> {
public:
    virtual ~CSSRule();

    enum class Type : u32 {
        Style,
        Import,
        Media,
        __Count,
    };

    virtual StringView class_name() const = 0;
    virtual Type type() const = 0;

    template<typename T>
    bool fast_is() const = delete;

private:
};

}
