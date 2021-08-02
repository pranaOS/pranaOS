/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/Singleton.h>
#include <kernel/devices/RandomDevice.h>
#include <kernel/net/NetworkAdapter.h>
#include <kernel/net/Routing.h>
#include <kernel/net/UDP.h>
#include <kernel/net/UDPSocket.h>
#include <kernel/Process.h>
#include <kernel/Random.h>


namespace Kernel {

void UDPSocket::for_each(Function<void(const UDPSocket&)> callback)
{
    MutexLocker locker(sockets_by_port().lock(), Mutex::Mode::Shared);
    for (auto it : sockets_by_port().resource())
        callback(*it.value);
}

static Base::Singleton<Lockable<HashMap<u16, UDPSocket*>>> s_map;

Lockable<HashMap<u16, UDPSocket*>>& UDPSocket::sockets_by_port()
{
    return *s_map;
}

SocketHandle<UDPSocket> UDPSocket::from_port(u16 port)
{
    RefPtr<UDPSocket> socket;
    {
        MutexLocker locker(sockets_by_port().lock(), Mutex::Mode::Shared);
        auto it = sockets_by_port().resource().find(port);
        if (it == sockets_by_port().resource().end())
            return {};
        socket = (*it).value;
        VERIFY(socket);
    }
    return { *socket };
}

UDPSocket::UDPSocket(int protocol)
    : IPv4Socket(SOCK_DGRAM, protocol)
{
}

UDPSocket::~UDPSocket()
{
    MutexLocker locker(sockets_by_port().lock());
    sockets_by_port().resource().remove(local_port());
}

KResultOr<NonnullRefPtr<UDPSocket>> UDPSocket::create(int protocol)
{
    auto socket = adopt_ref_if_nonnull(new (nothrow) UDPSocket(protocol));
    if (socket)
        return socket.release_nonnull();
    return ENOMEM;
}

KResultOr<size_t> UDPSocket::protocol_receive(ReadonlyBytes raw_ipv4_packet, UserOrKernelBuffer& buffer, size_t buffer_size, [[maybe_unused]] int flags)
{
    auto& ipv4_packet = *(const IPv4Packet*)(raw_ipv4_packet.data());
    auto& udp_packet = *static_cast<const UDPPacket*>(ipv4_packet.payload());
    VERIFY(udp_packet.length() >= sizeof(UDPPacket)); 
    size_t read_size = min(buffer_size, udp_packet.length() - sizeof(UDPPacket));
    if (!buffer.write(udp_packet.payload(), read_size))
        return EFAULT;
    return read_size;
}

}