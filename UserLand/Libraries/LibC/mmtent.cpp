/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


// includes
#include <AK/Format.h>
#include <assert.h>
#include <mntent.h>

extern "C" {

struct mntent* getmntent(FILE*)
{
    dbgln("FIXME: Implement getmntent()");
    TODO();
    return nullptr;
}
}