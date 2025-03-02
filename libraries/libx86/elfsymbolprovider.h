/**
 * @file elfsymbolprovider.h
 * @author Krisna Pranav
 * @brief ELF Symbol Provider
 * @version 6.0
 * @date 2024-11-07
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <libelf/image.h>
#include <libx86/instruction.h>

namespace X86 
{
    class ELFSymbolProvider final : public SymbolProvider 
    {
    public:
        /**
         * @brief Construct a new ELFSymbolProvider object
         * 
         * @param elf 
         * @param base_address 
         */
        ELFSymbolProvider(const ELF::Image& elf, FlatPtr base_address = 0)
            : m_elf(elf)
            , m_base_address(base_address)
        {
        }

        /**
         * @param address 
         * @param offset 
         * @return String 
         */
        virtual String symbolicate(FlatPtr address, u32* offset = nullptr) const override
        {
            return m_elf.symbolicate(address - m_base_address, offset);
        }

    private:
        const ELF::Image& m_elf;
        FlatPtr m_base_address;
    }; // class ELFSymbolProvider final : public SymbolProvider
} // namespace X86