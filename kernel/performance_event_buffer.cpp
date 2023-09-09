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
    {}
    
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
        case PERF_EVENT_FREE:
            event.data.free.ptr = arg1;
        default:
            return KResult(-EINVAL);
        }
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
}