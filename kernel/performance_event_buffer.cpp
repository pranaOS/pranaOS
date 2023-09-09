/**
 * @file performance_event_buffer.cpp
 * @author Krisna Pranav
 * @brief performance event buffer
 * @version 6.0
 * @date 2023-09-08
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <mods/jsonarrayserializer.h>
#include <mods/jsonobject.h>
#include <mods/jsonobjectserialize.h>
#include <kernel/kbufferbuilder.h>
#include <kernel/performance_event_buffer.h>

namespace Kernel 
{

    /// @brief Construct a new Performance Event Buffer:: Performance Event Buffer object
    PerformanceEventBuffer::PerformanceEventBuffer()
        : m_buffer(KBuffer::create_with_size(4 * MiB))
    { }

    /**
     * @param type 
     * @param arg1 
     * @param arg2 
     * @return KResult 
     */
    KResult PerformanceEventBuffer::append(int type, FlatPtr arg1, FlatPtr arg2)
    {
        if (count() >= capacity())
            return KResult(-ENOBUFS);

        PerformanceEvent event;
        event.type = type;

        switch (type) {
        case PERF_EVENT_MALLOC:
            event.data.malloc.size = arg1;
            event.data.malloc.ptr = arg2;
    #ifdef VERY_DEBUG
            dbg() << "PERF_EVENT_MALLOC: " << (void*)event.data.malloc.ptr << " (" << event.data.malloc.size << ")";
    #endif
            break;
        case PERF_EVENT_FREE:
            event.data.free.ptr = arg1;
    #ifdef VERY_DEBUG
            dbg() << "PERF_EVENT_FREE: " << (void*)event.data.free.ptr;
    #endif
            break;
        default:
            return KResult(-EINVAL);
        }

        FlatPtr ebp;

        asm volatile("movl %%ebp, %%eax"
                    : "=a"(ebp));

        auto current_thread = Thread::current();
        auto eip = current_thread->get_register_dump_from_stack().eip;

        Vector<FlatPtr> backtrace;
        {
            SmapDisabler disabler;
            backtrace = current_thread->raw_backtrace(ebp, eip);
        }
        event.stack_size = min(sizeof(event.stack) / sizeof(FlatPtr), static_cast<size_t>(backtrace.size()));
        memcpy(event.stack, backtrace.data(), event.stack_size * sizeof(FlatPtr));

    #ifdef VERY_DEBUG
        for (size_t i = 0; i < event.stack_size; ++i)
            dbg() << "    " << (void*)event.stack[i];
    #endif

        event.timestamp = TimeManagement::the().uptime_ms();
        at(m_count++) = event;

        return KSuccess;
    }

    /**
     * @param index 
     * @return PerformanceEvent& 
     */
    PerformanceEvent& PerformanceEventBuffer::at(size_t index)
    {
        ASSERT(index < capacity());
        auto* events = reinterpret_cast<PerformanceEvent*>(m_buffer.data());

        return events[index];
    }

    /**
     * @param pid 
     * @param executable_path 
     * @return KBuffer 
     */
    KBuffer PerformanceEventBuffer::to_json(ProcessID pid, const String& executable_path) const
    {
        KBufferBuilder builder;

        JsonObjectSerializer object(builder);
        object.add("pid", pid.value());
        object.add("executable", executable_path);

        auto array = object.add_array("events");

        for (size_t i = 0; i < m_count; ++i) {
            auto& event = at(i);
            auto event_object = array.add_object();

            switch (event.type) {
            case PERF_EVENT_MALLOC:
                event_object.add("type", "malloc");
                event_object.add("ptr", static_cast<u64>(event.data.malloc.ptr));
                event_object.add("size", static_cast<u64>(event.data.malloc.size));
                break;
            case PERF_EVENT_FREE:
                event_object.add("type", "free");
                event_object.add("ptr", static_cast<u64>(event.data.free.ptr));
                break;
            }

            event_object.add("timestamp", event.timestamp);
            auto stack_array = event_object.add_array("stack");

            for (size_t j = 0; j < event.stack_size; ++j) {
                stack_array.add(event.stack[j]);
            }

            stack_array.finish();
            event_object.finish();
        }

        array.finish();
        object.finish();

        return builder.build();
    }

} // namespace Kernel