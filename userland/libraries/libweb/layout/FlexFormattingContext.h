/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <libweb/layout/FormattingContext.h>

namespace Web::Layout {

class FlexFormattingContext final : public FormattingContext {
public:
    FlexFormattingContext(Box& containing_block, FormattingContext* parent);
    ~FlexFormattingContext();

    virtual void run(Box&, LayoutMode) override;
};

}
