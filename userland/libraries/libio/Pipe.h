/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

// includes
#include <libabi/Syscalls.h>
#include <libio/File.h>

namespace IO
{

struct Pipe
{
    RefPtr<Handle> reader;
    RefPtr<Handle> writer;

    static ResultOr<Pipe> create()
    {
        int reader_handle = HANDLE_INVALID_ID;
        int writer_handle = HANDLE_INVALID_ID;

        TRY(J_create_pipe(&reader_handle, &writer_handle));

        return Pipe{
            make<Handle>(reader_handle),
            make<Handle>(writer_handle),
        };
    }
};

}