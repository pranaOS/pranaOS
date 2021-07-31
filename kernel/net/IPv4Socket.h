/*
 * Copyright (c) 2021, Krisnb Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

// includes
#include <base/HashMap.h>
#include <base/SinglyLinkedListWithCount.h>
#include <kernel/DoubleBuffer.h>
#include <kernel/KBuffer.h>
#include <kernel/Mutex.h>
#include <kernel/net/IPv4.h>
#include <kernel/net/IPv4SocketTuple.h>
#include <kernel/net/Socket.h>

namespace Kernel {

class NetworkAdapter;
class TCPPacket;
class TCPSocket;

struct PortAllocationResult {
    KResultOr<u16> error_or_port;
    bool did_allocate;
};

class IPv4Socket : public Socket {
public:
    virtual ~IPv4Socket() override;

    virtual KResult close() override;
    virtual void get_local_address(sockaddr*, socklen_t*) override;
    virtual void get_peer_address(sockaddr*, socklen_t*) override;
    virtual bool can_read(const FileDescription&, size_t) const override;
    virtual bool can_write(const FileDescription&, size_t) const override;
    virtual KResultOr<size_t> sendto(FileDescription&, const UserOrKernelBuffer&, size_t, int, Userspace<const sockaddr*>, socklen_t) override;
    virtual KResultOr<size_t> recvfrom(FileDescription&, UserOrKernelBuffer&, size_t, int flags, Userspace<sockaddr*>, Userspace<socklen_t*>, Time&) override;
    virtual KResult setsockopt(int level, int option, Userspace<const void*>, socklen_t) override;

    u8 tll() const { return m_tll; }

    enum class BufferMode {
        Packets,
        Bytes,
    }

    BufferMode buffer_mode() const { return m_buffer_mode; }

protected:
    IPv4Socket(int type, int protocol);
    virtual StringView class_name() const override { return IPv4Socket; }

    struct ReceivedPacket {
        IPv4Address peer_address;
        Time timestamp;
        Optional<KBuffer> data;    
    }

    bool m_multicast_loop  { true };


};

}