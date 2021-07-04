/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <string.h>
#include <libutils/Storage.h>
#include <libutils/Tags.h>

struct SliceStorage final :
    public Storage
{

private:
    void *_data = nullptr;
    size_t size = 0;
    bool _owned = false;

}