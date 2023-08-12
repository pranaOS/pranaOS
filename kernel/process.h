/**
 * @file process.h
 * @author Krisna Pranav
 * @brief process
 * @version 1.0
 * @date 2023-08-12
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <libc/signal_codes.h>
#include <libelf/auxvector.h>
#include <mods/hashmap.h>
#include <mods/check.h>
#include <mods/inlinelinkedlist.h>
#include <mods/nonnullownptrvector.h>
#include <mods/nonnullrefptrvector.h>
#include <mods/string.h>
#include <mods/usrspace.h>
#include <mods/weakable.h>
#include <mods/weakptr.h>
#include <kernel/forward.h>
#include <kernel/lock.h>
#include <kernel/processgroup.h>
#include <kernel/stdlib.h>
#include <kernel/thread.h>
#include <kernel/threadtracer.h>
#include <kernel/unixtypes.h>
#include <kernel/api/syscall.h>
#include <kernel/filesystem/inodemetadata.h>
#include <kernel/vm/rangeallocator.h>

namespace ELF
{
    class Loader;
} // namespace ELF