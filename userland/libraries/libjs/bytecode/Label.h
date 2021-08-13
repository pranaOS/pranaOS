/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/Format.h>
#include <libjs/bytecode/BasicBlock.h>

namespace JS::Bytecode {

class Label {
public:
    explicit Label(BasicBlock const& block)
        : m_block(&block)
    {
    }

    auto& block() const { return *m_block; }

private:
    BasicBlock const* m_block { nullptr };
};

}

template<>
struct Base::Formatter<JS::Bytecode::Label> : Base::Formatter<FormatString> {
    void format(FormatBuilder& builder, JS::Bytecode::Label const& value)
    {
        return Base::Formatter<FormatString>::format(builder, "@{}", value.block().name());
    }
};
