/**
 * @file processor.h
 * @author Krisna Pranav
 * @brief Processor
 * @version 6.0
 * @date 2024-09-21
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/function.h>
#include <kernel/arch/deferredcallentry.h>

#if ARCH(X86_64) || ARCH(I386)
#    include <kernel/arch/x86/processor.h>
#elif ARCH(AARCH64)
#    include <kernel/arch/aarch64/processor.h>
#else
#    error "Unknown architecture"
#endif

namespace Kernel 
{
    namespace Memory 
    {
        class PageDirectory;
    } // namespace Memory

    struct ProcessorMessageEntry;
    struct ProcessorMessage 
    {
        using CallbackFunction = Function<void()>;

        enum Type 
        {
            FlushTlb,
            Callback,
        };

        Type type;
        Atomic<u32> refs;
        union 
        {
            ProcessorMessage* next; 
            alignas(CallbackFunction) u8 callback_storage[sizeof(CallbackFunction)];

            struct 
            {
                Memory::PageDirectory const* page_directory;
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
    }; // struct ProcessorMessage

    struct ProcessorMessageEntry 
    {
        ProcessorMessageEntry* next;
        ProcessorMessage* msg;
    }; // struct ProcessorMessageEntry

    template<typename T>
    class ProcessorSpecific 
    {
    public:
        static void initialize()
        {
            Processor::current().set_specific(T::processor_specific_data_id(), new T);
        }
        
        /**
         * @return T& 
         */
        static T& get()
        {
            return *Processor::current().get_specific<T>();
        }
    }; // class ProcessorSpecific
} // namespace Kernel