/**
 * @file singleton.h
 * @author Krisna Pranav
 * @brief Singleton
 * @version 1.0
 * @date 2023-07-26
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include "kmalloc.h"
#include "assertions.h"
#include "atomic.h"
#include "noncopyable.h"

#ifndef KERNEL
#   include <kernel/arch/i386/cpu.h>
#endif

#ifndef __prana__
#   include <new>
#endif

namespace Mods
{
    
}