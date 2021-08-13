/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <base/String.h>
#include <libjs/Bytecode/BasicBlock.h>
#include <libjs/Bytecode/Op.h>
#include <sys/mman.h>

namespace JS::Bytecode {

NonnullOwnPtr<BasicBlock> BasicBlock::create(String name, size_t size)
{
    return adopt_own(*new BasicBlock(move(name), max(size, static_cast<size_t>(4 * KiB))));
}

BasicBlock::BasicBlock(String name, size_t size)
    : m_name(move(name))
{
    m_buffer_capacity = size;
    m_buffer = (u8*)mmap(nullptr, m_buffer_capacity, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);
    VERIFY(m_buffer != MAP_FAILED);
}

BasicBlock::~BasicBlock()
{
    Bytecode::InstructionStreamIterator it(instruction_stream());
    while (!it.at_end()) {
        auto& to_destroy = (*it);
        ++it;
        Instruction::destroy(const_cast<Instruction&>(to_destroy));
    }

    munmap(m_buffer, m_buffer_capacity);
}

void BasicBlock::seal()
{
}

void BasicBlock::dump(Bytecode::Executable const& executable) const
{
    Bytecode::InstructionStreamIterator it(instruction_stream());
    if (!m_name.is_empty())
        warnln("{}:", m_name);
    while (!it.at_end()) {
        warnln("[{:4x}] {}", it.offset(), (*it).to_string(executable));
        ++it;
    }
}

void BasicBlock::grow(size_t additional_size)
{
    m_buffer_size += additional_size;
    VERIFY(m_buffer_size <= m_buffer_capacity);
}

void InstructionStreamIterator::operator++()
{
    VERIFY(!at_end());
    m_offset += dereference().length();
}

}
