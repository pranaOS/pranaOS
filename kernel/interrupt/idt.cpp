/**
* @file idt.cpp
* @author Krisna Pranav
* @brief IDT
* @version 1.0
*
* @copyright Copyright (c) 2023 pranaOS Developers, Krisna Pranav
*
 */

#include "../kstd/kstddef.h"
#include "../kstd/cstring.h"
#include "idt.h"

struct Interrupt:IDTPointer idtp;