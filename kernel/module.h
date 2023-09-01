/**
 * @file module.h
 * @author Krisna Pranav
 * @brief module
 * @version 6.0
 * @date 2023-09-01
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <kernel/kbuffer.h>
#include <mods/string.h>
#include <mods/vector.h>

namespace Kernel 
{

    typedef void* (*ModuleInitPtr)();
    typedef void* (*ModuleFiniPtr)();

    struct Module 
    {
        String name;
        Vector<KBuffer> sections;

        ModuleInitPtr module_init { nullptr };
        ModuleFiniPtr module_fini { nullptr };
    }; // struct Module
} // namespace Kernel