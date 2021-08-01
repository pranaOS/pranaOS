/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <kernel/net/NetworkAdapter.h>

namespace Kernel {

class LoopbackAdapter final : public NetworkAdapter {
    AK_MAKE_ETERNAL

private:
    LoopbackAdapter();

public:
    static RefPtr<LoopbackAdapter> try_create();
    virtual ~LoopbackAdapter() override;

    virtual void send_raw(ReadonlyBytes) override;
    virtual StringView class_name() const override { return "LoopbackAdapter"; }
    virtual bool link_up() override { return true; }
};

}