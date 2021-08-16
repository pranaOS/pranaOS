/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <libtest/TestCase.h>
#include <base/Badge.h>

TEST_CASE(should_provide_underlying_type)
{
    static_assert(IsSame<int, Badge<int>::Type>);
}
