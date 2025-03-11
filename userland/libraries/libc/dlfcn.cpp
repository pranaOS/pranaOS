/**
 * @file dlfcn.cpp
 * @author Krisna Pranav
 * @brief dlfcn
 * @version 6.0
 * @date 2025-02-23
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#include <libdl/dlfcn_integration.h>

DlCloseFunction __dlclose;
DlOpenFunction __dlopen;
DlSymFunction __dlsym;
DlAddrFunction __dladdr;
