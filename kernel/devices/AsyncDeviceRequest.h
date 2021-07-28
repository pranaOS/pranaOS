/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/IntrusiveList.h>
#include <base/NonnullRefPtr.h>
#include <kernel/Process.h>
#include <kernel/Thread.h>
#include <kernel/UserOrKernelBuffer.h>
#include <kernel/vm/ProcessPagingScope.h>
#include <kernel/WaitQueue.h>

namespace Kernel {

class Device;

extern WorkQueue* g_io_work;

class AsyncDeviceRequest : public RefCounted<AsyncDeviceRequest> {
    BASE_MAKE_NONCOPYABLE(AsyncDeviceRequest);
    BASE_MAKE_NONMOVABLE(AsyncDeviceRequest);

public:
    enum [[nodiscard]] RequestResult {
        Pending = 0,
        Started,
        Success,
        Failure,
        MemoryFault,
        Cancelled
    };

    class RequestWaitResult {
        friend class AsyncDeviceRequest;

    public:
        RequestResult request_result() const { return m_request_result; }
        Thread::BlockResult wait_result() const { return m_wait_result; }

    private:
        RequestWaitResult(RequestResult request_result, Thread::BlockResult wait_result)
            : m_request_result(request_result)
            , m_wait_result(wait_result)
        {
        }

        RequestResult m_request_result;
        Thread::BlockResult m_wait_result;
    };

    virtual ~AsyncDeviceRequest();

    virtual const char* name() const = 0;
    virtual void start() = 0;

    void add_sub_request(NonnullRefPtr<AsyncDeviceRequest>);

    [[nodiscard]] RequestWaitResult wait(Time* = nullptr);

}