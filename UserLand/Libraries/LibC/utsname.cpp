/*
 * Copyright (c) 2021 krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


// includes
#include <errno.h>
#include <sys/utsname.h>
#include <syscall.h>

extern "C" {
// uname
int uname(struct utsname* buf)
{
    int rc = syscall(SC_uname, buf);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}
}
