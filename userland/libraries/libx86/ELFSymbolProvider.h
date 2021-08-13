/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libelf/Image.h>
#include <libx86/Instruction.h>

namespace X86 {

class ELFSymbolProvider final : public SymbolProvider {
public:
    ELFSymbolProvider(const ELF::Image& elf)
        : m_elf(elf)
    {
    }

    virtual String symbolicate(FlatPtr address, u32* offset = nullptr) const override
    {
        return m_elf.symbolicate(address, offset);
    }

private:
    const ELF::Image& m_elf;
};
}
