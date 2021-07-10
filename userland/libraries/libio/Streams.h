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

    

};

}