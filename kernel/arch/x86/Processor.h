/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Array.h>
#include <base/Concepts.h>
#include <base/Function.h>
#include <base/Types.h>
#include <kernel/arch/x86/ASM_wrapper.h>
#include <kernel/arch/x86/CPUID.h>
#include <kernel/arch/x86/DescriptorTable.h>
#include <kernel/arch/x86/PageDirectory.h>
#include <kernel/arch/x86/TSS.h>
#include <kernel/Forward.h>

namespace Kernel {

class ProcessorInfo;
class SchedulerPerProcessorData;
struct MemoryManagerData;
struct ProcessorMessageEntry;

#if ARCH(X86_64)
#    define MSR_FS_BASE 0xc0000100
#    define MSR_GS_BASE 0xc0000101
#endif

extern "C" void thread_context_first_enter(void);
extern "C" void exit_kernel_thread(void);
extern "C" void do_assume_context(Thread* thread, u32 flags);

struct [[gnu::aligned(16)]] FPUState
{
    u8 buffer[512];
};

struct ProcessorMessage {
    using CallbackFunction = Function<void()>;

    enum Type {
        FlushTlb,
        Callback,
    };
    Type type;
    Atomic<u32> refs;
    union {
        ProcessorMessage* next; 
        alignas(CallbackFunction) u8 callback_storage[sizeof(CallbackFunction)];
        struct {
            const PageDirectory* page_directory;
            u8* ptr;
            size_t page_count;
        } flush_tlb;
    };

    volatile bool async;

    ProcessorMessageEntry* per_proc_entries;

    CallbackFunction& callback_value()
    {
        return *bit_cast<CallbackFunction*>(&callback_storage);
    }

    void invoke_callback()
    {
        VERIFY(type == Type::Callback);
        callback_value()();
    }
};

struct ProcessorMessageEntry {
    ProcessorMessageEntry* next;
    ProcessorMessage* msg;
};

struct DeferredCallEntry {
    using HandlerFunction = Function<void()>;

    DeferredCallEntry* next;
    alignas(HandlerFunction) u8 handler_storage[sizeof(HandlerFunction)];
    bool was_allocated;

    HandlerFunction& handler_value()
    {
        return *bit_cast<HandlerFunction*>(&handler_storage);
    }

    void invoke_handler()
    {
        handler_value()();
    }
};