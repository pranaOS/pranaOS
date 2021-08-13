/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <base/Badge.h>
#include <libjs/heap/BlockAllocator.h>
#include <libjs/heap/CellAllocator.h>
#include <libjs/heap/Heap.h>
#include <libjs/heap/HeapBlock.h>

namespace JS {

CellAllocator::CellAllocator(size_t cell_size)
    : m_cell_size(cell_size)
{
}

CellAllocator::~CellAllocator()
{
}

Cell* CellAllocator::allocate_cell(Heap& heap)
{
    if (m_usable_blocks.is_empty()) {
        auto block = HeapBlock::create_with_cell_size(heap, m_cell_size);
        m_usable_blocks.append(*block.leak_ptr());
    }

    auto& block = *m_usable_blocks.last();
    auto* cell = block.allocate();
    VERIFY(cell);
    if (block.is_full())
        m_full_blocks.append(*m_usable_blocks.last());
    return cell;
}

void CellAllocator::block_did_become_empty(Badge<Heap>, HeapBlock& block)
{
    auto& heap = block.heap();
    block.m_list_node.remove();

    block.~HeapBlock();
    heap.block_allocator().deallocate_block(&block);
}

void CellAllocator::block_did_become_usable(Badge<Heap>, HeapBlock& block)
{
    VERIFY(!block.is_full());
    m_usable_blocks.append(block);
}

}
