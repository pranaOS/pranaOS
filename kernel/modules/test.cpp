/**
 * @file test.cpp
 * @author Krisna Pranav
 * @brief test
 * @version 6.0
 * @date 2023-07-12
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/logstream.h>
#include "syms.h"

extern "C" const char module_name[] = "testModule";

extern "C" void module_init()
{
    klog() << "TEST: Module has started";

    for (int i = 0; i < 3; ++i) {
        klog() << "value of i is " << i;
    }
}

extern "C" void module_fini()
{
    klog() << "TEST: destroying the module";
}