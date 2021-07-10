/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libio/BufLine.h>
#include <libio/Format.h>
#include <libio/Handle.h>
#include <libsystem/process/Process.h>

namespace IO
{


struct InStream :
    public Reader,
    public RawHandle
{

private:
    RefPtr<Handle> _handle;

public:
    using Reader::read;

    InStream() : _handle{make<Handle>{0}} {}

    ResultOr<size_t> read(void *buffer, size_t size) override { return _handle->read(buffer, size); }
    RefPtr<Handle> handle() override { return _handle; }

};


struct OutStream :
    public Writer,
    public RawHandle
{
private:
    RefPtr<Handle> _handle;

public:
    using Writer::write;

    OutStream() : _handle{make<Handle>(1)} {}

    ResultOr<size_t> write(const void *buffer, size_t size) override { return _handle->write(buffer, size); }
    RefPtr<Handle> handle() override { return _handle; }
};


}