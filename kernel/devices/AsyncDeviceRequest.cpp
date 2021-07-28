/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <kernel/devices/AsyncDeviceRequest.h>
#include <kernel/devices/Device.h>

namespace Kernel {

AsyncDeviceRequest::AsyncDeviceRequest(Device& device)
    : m_device(device)
    , m_process(*Process::current())
{
}

AsyncDeviceRequest::~AsyncDeviceRequest()
{
    {
        ScopedSpinLock lock(m_lock);
        VERIFY(is_completed_result(m_result));
        VERIFY(m_sub_requests_pending.is_empty());
    }

    while (!m_sub_requests_complete.is_empty()) {

        auto sub_request = m_sub_requests_complete.take_first();
        VERIFY(is_completed_result(sub_request->m_result)); 
        VERIFY(sub_request->m_parent_request == this);
        sub_request->m_parent_request = nullptr;
    }
}

void AsyncDeviceRequest::request_finished()
{
    if (m_parent_request)
        m_parent_request->sub_request_finished(*this);

    m_device.process_next_queued_request({}, *this);

    m_queue.wake_all();
}

auto AsyncDeviceRequest::wait(Time* timeout) -> RequestWaitResult
{
    VERIFY(!m_parent_request);
    auto request_result = get_request_result();
    if (is_completed_result(request_result))
        return { request_result, Thread::BlockResult::NotBlocked };
    auto wait_result = m_queue.wait_on(Thread::BlockTimeout(false, timeout), name());
    return { get_request_result(), wait_result };
}

auto AsyncDeviceRequest::get_request_result() const -> RequestResult
{
    ScopedSpinLock lock(m_lock);
    return m_result;
}

void AsyncDeviceRequest::add_sub_request(NonnullRefPtr<AsyncDeviceRequest> sub_request)
{
    VERIFY(&m_device != &sub_request->m_device);
    VERIFY(sub_request->m_parent_request == nullptr);
    sub_request->m_parent_request = this;

    ScopedSpinLock lock(m_lock);
    VERIFY(!is_completed_result(m_result));
    m_sub_requests_pending.append(sub_request);
    if (m_result == Started)
        sub_request->do_start(move(lock));
}

void AsyncDeviceRequest::sub_request_finished(AsyncDeviceRequest& sub_request)
{
    bool all_completed;
    {
        ScopedSpinLock lock(m_lock);
        VERIFY(m_result == Started);

        if (m_sub_requests_pending.contains(sub_request)) {
            m_sub_requests_complete.append(sub_request);
        }

        all_completed = m_sub_requests_pending.is_empty();
        if (all_completed) {
            bool any_failures = false;
            bool any_memory_faults = false;
            for (auto& com_sub_request : m_sub_requests_complete) {
                auto sub_result = com_sub_request.get_request_result();
                VERIFY(is_completed_result(sub_result));
                switch (sub_result) {
                case Failure:
                    any_failures = true;
                    break;
                case MemoryFault:
                    any_memory_faults = true;
                    break;
                default:
                    break;
                }
                if (any_failures && any_memory_faults)
                    break; 
            }
            if (any_failures)
                m_result = Failure;
            else if (any_memory_faults)
                m_result = MemoryFault;
            else
                m_result = Success;
        }
    }
    if (all_completed)
        request_finished();
}

void AsyncDeviceRequest::complete(RequestResult result)
{
    VERIFY(result == Success || result == Failure || result == MemoryFault);
    ScopedCritical critical;
    {
        ScopedSpinLock lock(m_lock);
        VERIFY(m_result == Started);
        m_result = result;
    }
    if (Processor::current().in_irq()) {
        ref(); 
        Processor::deferred_call_queue([this]() {
            request_finished();
            unref();
        });
    } else {
        request_finished();
    }
}

}