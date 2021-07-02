/*
 * Copyright (c) 2021 krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <AK/Format.h>
#include <assert.h>
#include <wctype.h>

extern "C" {

wctype_t wctype(const char*)
{
    dbgln("FIXME: Implement wctype()");
    TODO();
}

int iswctype(wint_t, wctype_t)
{
    dbgln("FIXME: Implement iswctype()");
    TODO();
}
}
