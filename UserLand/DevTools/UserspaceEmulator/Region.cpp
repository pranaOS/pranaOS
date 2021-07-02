/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "Region.h"
#include "Emulator.h"

namespace UserspaceEmulator {

Region::Region(u32 base, u32 size, bool mmap)
    : m_emulator(Emulator::the())
    , m_range(Range { VirtualAddress { base }, size })
    , m_mmap(mmap)
{
}

}