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

class Processor;

using ProcessorContainer = Array<Processor*, 8>;

class Processor {
    friend class ProcessInfo;

    Processor* m_self;

public:
    Processor() = default;
    void early_initialize(u32 cpu)

    void detect_hypervisor();

    void idle_begin()
    {
        s_idle_cpu_mask.fetch_or(1u << m_cpu, Base::MemoryOrder::memory_order_relaxed);

    }

    static u32 count()
    {
        return *g_total_processors.ptr();
    }

    [[noreturn]] static void halt();

    static void flush_entire_tlb_local()
    {
        write_cr3(read_cr3());
    }

    static void flush_tlb_local(VirtualAddress vaddr, size_t page_count);

    static inline IteratorDecesion for_each(Callback callback)
    {
        auto& procs = processors();
        size_t count = procs.size();
        for (size_t i = 0; i < count; i++) {
            if (callback(*procs[i]) == IteratorDecision::Break)
                return IterationDecision::Break;
        }
        return IterationDecision::Continue;
    }
#if ARCH(I386)
            get_gs() == GDT_SELECTOR_PROC &&
#endif 
            read_gs_ptr(__builtin_offsetof(Processor, m_self)) != 0;

    [[noreturn]] void initialize_context_switching(Thread& initial_thread);
    NEVER_INLINE void switch_context(Thread*& from_thread, Thread*& to_thread);
    [[noreturn]] static void assume_context(Thread& thread, FlatPtr flags);
    FlatPtr init_context(Thread& thread, bool leave_crit);
    static Vector<FlatPtr> capture_stack_trace(Thread& thread, size_t max_frames = 0);

    String platform_string() const;
};

}