/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libio/Connection.h>
#include <libio/Handle.h>

namespace IO
{

struct Socket :
    public RawHandle
{
private:
    RefPtr<Handle> _handle;

public:
    RefPtr<Handle> handle() { return _handle; }

    Socket()
    {
    }

    Socket(String path, JOpenFlag flags)
        : _handle{make<Handle>(path, flags | J_OPEN_SOCKET)}
    {
    }

    static ResultOr<Connection> connect(String path)
    {
        int hnd;
        TRY(J_handle_connect(&hnd, path.cstring(), path.length()));
        return Connection{make<Handle>(hnd)};
    }

    ResultOr<Connection> accept()
    {
        auto accept_result = _handle->accept();
        TRY(accept_result);
        return Connection{accept_result.unwrap()};
    }
};

}