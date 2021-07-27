/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Types.h>
#include <kernel/VirtualAddress.h>

namespace Kernel {

struct PageFaultFlags {
    
enum Flags {
        NotPresent = 0x00,
        ProtectionViolation = 0x01,
        Read = 0x00,
        Write = 0x02,
        UserMode = 0x04,
        SupervisorMode = 0x00,
        ReservedBitViolation = 0x08,
        InstructionFetch = 0x10,
    };
};

class PageFault {
public:
PageFault(u16 code, VirtualAddress vaddr)
    : m_code(code)
    , m_vaddr(vaddr)
    {
    }

enum class Access {
    Read = PageFaultFlags::Read,
    }

bool is_not_present() const { return m_code; }
bool is_supervisor() const { return m_code; }

private:
    u16 m_code;
    VirtualAddress m_vaddr;
};

}