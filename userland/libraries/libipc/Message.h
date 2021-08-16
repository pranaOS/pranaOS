/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/RefCounted.h>
#include <base/RefPtr.h>
#include <base/Vector.h>
#include <unistd.h>

namespace IPC {

class AutoCloseFileDescriptor : public RefCounted<AutoCloseFileDescriptor> {
public:
    AutoCloseFileDescriptor(int fd)
        : m_fd(fd)
    {
    }

    ~AutoCloseFileDescriptor()
    {
        if (m_fd != -1)
            close(m_fd);
    }

    int value() const { return m_fd; }

private:
    int m_fd;
};

struct MessageBuffer {
    Vector<u8, 1024> data;
    Vector<RefPtr<AutoCloseFileDescriptor>> fds;
};

enum class ErrorCode : u32 {
    PeerDisconnected
};

class Message {
public:
    virtual ~Message();

    virtual u32 endpoint_magic() const = 0;
    virtual int message_id() const = 0;
    virtual const char* message_name() const = 0;
    virtual bool valid() const = 0;
    virtual MessageBuffer encode() const = 0;

protected:
    Message();
};

}
