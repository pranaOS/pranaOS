/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/Singleton.h>
#include <base/Time.h>
#include <kernel/Debug.h>
#include <kernel/devices/RandomDevice.h>
#include <kernel/filesystem/FileDescription.h>
#include <kernel/net/EthernetFrameHeader.h>
#include <kernel/net/IPv4.h>
#include <kernel/net/NetworkAdapter.h>
#include <kernel/net/NetworkingManagement.h>
#include <kernel/net/Routing.h>
#include <kernel/net/TCP.h>
#include <kernel/net/TCPSocket.h>
#include <kernel/Process.h>
#include <kernel/Random.h>

namespace Kernel {

void TCPSocket::for_each(Function<void(const TCPSocket&)> callback)
{
    MutexLocker locker(sockets_by_tuple().lock(), Mutex::Mode::Shared);
    for (auto& it : sockets_by_tuple().resource())
        callback(*it.value);
}

void TCPSocket::set_state(State new_state)
{
    dbgln_if(TCP_SOCKET_DEBUG, "TCPSocket({}) state moving from {} to {}", this, to_string(m_state), to_string(new_state));

    auto was_disconnected = protocol_is_disconnected();
    auto previous_role = m_role;

    m_state = new_state;

    if (new_state == State::Established && m_direction == Direction::Outgoing)
        m_role = Role::Connected;

    if (new_state == State::Closed) {
        MutexLocker locker(closing_sockets().lock());
        closing_sockets().resource().remove(tuple());

        if (m_originator)
            release_to_originator();
    }

    if (previous_role != m_role || was_disconnected != protocol_is_disconnected())
        evaluate_block_conditions();
}

static Base::Singleton<Lockable<HashMap<IPv4SocketTuple, RefPtr<TCPSocket>>>> s_socket_closing;

Lockable<HashMap<IPv4SocketTuple, RefPtr<TCPSocket>>>& TCPSocket::closing_sockets()
{
    return *s_socket_closing;
}

static Base::Singleton<Lockable<HashMap<IPv4SocketTuple, TCPSocket*>>> s_socket_tuples;

Lockable<HashMap<IPv4SocketTuple, TCPSocket*>>& TCPSocket::sockets_by_tuple()
{
    return *s_socket_tuples;
}

RefPtr<TCPSocket> TCPSocket::from_tuple(const IPv4SocketTuple& tuple)
{
    MutexLocker locker(sockets_by_tuple().lock(), Mutex::Mode::Shared);

    auto exact_match = sockets_by_tuple().resource().get(tuple);
    if (exact_match.has_value())
        return { *exact_match.value() };

    auto address_tuple = IPv4SocketTuple(tuple.local_address(), tuple.local_port(), IPv4Address(), 0);
    auto address_match = sockets_by_tuple().resource().get(address_tuple);
    if (address_match.has_value())
        return { *address_match.value() };

    auto wildcard_tuple = IPv4SocketTuple(IPv4Address(), tuple.local_port(), IPv4Address(), 0);
    auto wildcard_match = sockets_by_tuple().resource().get(wildcard_tuple);
    if (wildcard_match.has_value())
        return { *wildcard_match.value() };

    return {};
}
RefPtr<TCPSocket> TCPSocket::create_client(const IPv4Address& new_local_address, u16 new_local_port, const IPv4Address& new_peer_address, u16 new_peer_port)
{
    auto tuple = IPv4SocketTuple(new_local_address, new_local_port, new_peer_address, new_peer_port);

    {
        MutexLocker locker(sockets_by_tuple().lock(), Mutex::Mode::Shared);
        if (sockets_by_tuple().resource().contains(tuple))
            return {};
    }

    auto result = TCPSocket::create(protocol());
    if (result.is_error())
        return {};

    auto client = result.release_value();
    client->set_setup_state(SetupState::InProgress);
    client->set_local_address(new_local_address);
    client->set_local_port(new_local_port);
    client->set_peer_address(new_peer_address);
    client->set_peer_port(new_peer_port);
    client->set_direction(Direction::Incoming);
    client->set_originator(*this);

    MutexLocker locker(sockets_by_tuple().lock());
    m_pending_release_for_accept.set(tuple, client);
    sockets_by_tuple().resource().set(tuple, client);

    return client;
}

void TCPSocket::release_to_originator()
{
    VERIFY(!!m_originator);
    m_originator.strong_ref()->release_for_accept(this);
    m_originator.clear();
}

void TCPSocket::release_for_accept(RefPtr<TCPSocket> socket)
{
    VERIFY(m_pending_release_for_accept.contains(socket->tuple()));
    m_pending_release_for_accept.remove(socket->tuple());
    [[maybe_unused]] auto rc = queue_connection_from(*socket);
}

TCPSocket::TCPSocket(int protocol)
    : IPv4Socket(SOCK_STREAM, protocol)
{
    m_last_retransmit_time = kgettimeofday();
}

}