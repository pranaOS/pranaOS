/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <LibDl/dlfcn_integration.h>

DlCloseFunction __dlclose;
DlOpenFunction __dlopen;
DlSymFunction __dlsym;
DlAddrFunction __dladdr;
