/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/NonnullRefPtrVector.h>
#include <base/RefCounted.h>
#include <base/RefPtr.h>
#include <base/Time.h>
#include <kernel/filesystem/File.h>
#include <kernel/KResult.h>
#include <kernel/Mutex.h>
#include <kernel/net/NetworkAdapter.h>
#include <kernel/UnixTypes.h>


namespace Kernel {

enum class ShouldBlock {
    No = 0,
    Yes = 1,
};

class FileDescription;

class Socket : public File {
public:
    static KResultOr<NonnullRefPtr<Socket>> create(int domain, int type, int protocol);
    virtual ~Socket() override;

    int domain() const { return m_domain; }
    int type() const { return m_type; }
    int protocol() const { return m_protocol; }
    
    bool is_shut_down_for_writing() const { return m_shut_down_for_writing; }
    bool is_shutdown_for_reading() const { return m_shut_down_for_reading; }

protected:
    ucred m_origin { 0, 0, 0 };
    ucred m_acceptor { 0, 0, 0 };

private:
    virtual bool is_socket() const final { return true; }
};

template<typename SocketType>
class SocketHandle {
public:
    SocketHandle() = default;

    SocketHandle(NonnullRefPtr<SocketType>&& socket)
        : m_socket(move(socket))
    {
        if (m_socket)
            m_socket->lock().lock();
    }

    SocketHandle(SocketHandle&& other)
        : m_socket(move(other.m_socket))
    {
    }

    ~SocketHandle()
    {
        if (m_socket)
            m_socket->lock().unblock();
    }

    SocketHandle(const SocketHandle&) = delete;
    SocketHandle& operator=(const SocketHandle&) = delete;

    operator bool() const { return m_socket; }

    SocketType* operator->() { return &socket(); }
    const SocketType* operator->() { return &socket(); }

};

}
