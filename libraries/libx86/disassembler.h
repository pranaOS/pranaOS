/**
 * @file disassembler.h
 * @author Krisna Pranav
 * @brief Disassembler
 * @version 6.0
 * @date 2024-11-07
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/optional.h>
#include <libx86/instruction.h>

namespace X86 
{
    class Disassembler 
    {
    public:
        /**
         * @brief Construct a new Disassembler object
         * 
         * @param stream 
         */
        explicit Disassembler(InstructionStream& stream)
            : m_stream(stream)
        {
        }

        /**
         * @return Optional<Instruction> 
         */
        Optional<Instruction> next()
        {
            if (!m_stream.can_read())
                return {};
    #if ARCH(I386)
            return Instruction::from_stream(m_stream, true, true);
    #else
            dbgln("FIXME: Implement disassembly support for x86 architecture bro :)");
            return {};
    #endif
        }

    private:
        InstructionStream& m_stream;
    }; // class Disassembler
} // namespace X86