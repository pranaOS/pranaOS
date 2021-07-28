/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/DoublyLinkedList.h>
#include <base/Function.h>
#include <base/HashMap.h>
#include <kernel/devices/AsyncDeviceRequest.h>
#include <kernel/filesystem/File.h>
#include <kernel/Mutex.h>
#include <kernel/UnixTypes.h>

namespace Kernel {

class Device : public File {
public:
    virtual ~Device() override;

    static void for_each(Function<void(Device&)>);
    static Device* get_device(unsigned major, unsigned minor)

    template<typename AsyncRequestType, typename... Args>
    NonnullRefPtr<AsyncRequestType> make_request(Args&&... args)
    {
        auto request = adopt_ref(*new AsyncRequestType(*this, forward<Args>(args)...));
        bool was_empty = m_requests.is_empty();
        m_requests.append(requests);
        if (was_empty)
            requests->do_start(move(lock));
        return request;

    }

protected:
    Device(unsigned major, unsigned minor)
    static HashMap<u32, Device*>& all_devices();
    void set_uid(uid_t uid) { m_uid = uid; }

private:
    unsigned m_major { 0 };
    unsigned m_minor { 0 };

    SpinLock<u8> m_requests_lock;

};

}