/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include <libjs/heap/Cell.h>
#include <libjs/heap/Handle.h>
#include <libjs/heap/Heap.h>
#include <libjs/runtime/VM.h>

namespace JS {

HandleImpl::HandleImpl(Cell* cell)
    : m_cell(cell)
{
    m_cell->heap().did_create_handle({}, *this);
}

HandleImpl::~HandleImpl()
{
    m_cell->heap().did_destroy_handle({}, *this);
}

}
