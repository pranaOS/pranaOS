/*
 * Copyright (c) 2021, krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libutils/Hash.h>
#include <libutils/RefPtr.h>
#include <libutils/Slice.h>
#include <libutils/Std.h>
#include <libutils/StringStorage.h>


namespace Utils
{

struct String :
    public RawStorage
{
private:
    RefPtr<StringStorage> _storage;


}

}