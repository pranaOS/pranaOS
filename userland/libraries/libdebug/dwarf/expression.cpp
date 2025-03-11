/**
 * @file expression.cpp
 * @author Krisna Pranav
 * @brief expression
 * @version 6.0
 * @date 2025-03-09
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */


#include "expression.h"
#include <mods/format.h>
#include <mods/memorystream.h>
#include <sys/arch/i386/regs.h>

namespace Debug::Dwarf::Expression 
{

    /**
     * @param bytes 
     * @param regs 
     * @return Value 
     */
    Value evaluate(ReadonlyBytes bytes, [[maybe_unused]] PtraceRegisters const& regs)
    {
        InputMemoryStream stream(bytes);

        while (!stream.eof()) {
            u8 opcode = 0;
            stream >> opcode;

            switch (static_cast<Operations>(opcode)) {
    #if ARCH(I386)
            case Operations::RegEbp: {
                ssize_t offset = 0;
                stream.read_LEB128_signed(offset);
                return Value { Type::UnsignedInteger, { regs.ebp + offset } };
            }

            case Operations::FbReg: {
                ssize_t offset = 0;
                stream.read_LEB128_signed(offset);
                return Value { Type::UnsignedInteger, { regs.ebp + 2 * sizeof(size_t) + offset } };
            }
    #endif

            default:
                dbgln("DWARF expr addr: {:p}", bytes.data());
                dbgln("unsupported opcode: {}", opcode);
                VERIFY_NOT_REACHED();
            }
        }
        VERIFY_NOT_REACHED();
    }

} // namespace Debug::Dwarf::Expression
