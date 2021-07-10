/*
 * Copyright (c) 2021, krishpranav
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



};

}