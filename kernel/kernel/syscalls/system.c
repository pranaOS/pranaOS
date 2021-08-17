/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <libkernel/bits/errno.h>
#include <libkernel/libkern.h>
#include <libkernel/log.h>
#include <libkernel/version.h>
#include <mem/vmm/vmm.h>
#include <platform/generic/syscalls/params.h>
#include <syscalls/handlers.h>

void sys_uname(trapframe_t* tf)
{
    utsname_t* buf = (utsname_t*)param1;
    vmm_copy_to_user(buf->sysname, OSTYPE, sizeof(OSTYPE));
    vmm_copy_to_user(buf->release, OSRELEASE, sizeof(OSRELEASE));
    vmm_copy_to_user(buf->version, VERSION_VARIANT, sizeof(VERSION_VARIANT));
    vmm_copy_to_user(buf->machine, MACHINE, sizeof(MACHINE));
    return_with_val(0);
}