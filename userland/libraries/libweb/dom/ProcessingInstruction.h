/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include <base/FlyString.h>
#include <libweb/dom/CharacterData.h>

namespace Web::DOM {

class ProcessingInstruction final : public CharacterData {
public:
    using WrapperType = Bindings::ProcessingInstructionWrapper;

    ProcessingInstruction(Document&, const String& data, const String& target);
    virtual ~ProcessingInstruction() override;

    virtual FlyString node_name() const override { return m_target; }

    const String& target() const { return m_target; }

private:
    String m_target;
};

}
