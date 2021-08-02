/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Function.h>
#include <base/HashMap.h>
#include <base/SinglyLinkedList.h>
#include <base/WeakPtr.h>
#include <kernel/KResult.h>
#include <kernel/net/IPv4Socket.h>


namespace Kernel {

class TCPSocket final : public IPv4Socket {
public:
    static void for_each(Function<void(const TCPSocket&)>);
    static KResultOr<NonnullRefPtr<TCPSocket>> create(int protocol);
    virtual ~TCPSocket() override;

    enum class Direction {
        Unspecified,
        Outgoing,
        Incoming,
        Passive,
    };

    static const char* to_string(Direction direction)
    {
        switch (direction) {
        case Direction::Unspecified:
            return "Unspecified";
        case Direction::Outgoing:
            return "Outgoing";
        case Direction::Incoming:
            return "Incoming";
        case Direction::Passive:
            return "Passive";
        default:
            return "None";
        }
    }

    enum class State {
        Closed,
        Listen,
        SynSent,
        SynReceived,
        Established,
        CloseWait,
        LastAck,
        FinWait1,
        FinWait2,
        Closing,
        TimeWait,
    };

    static const char* to_string(State state)
    {
        switch (state) {
        case State::Closed:
            return "Closed";
        case State::Listen:
            return "Listen";
        case State::SynSent:
            return "SynSent";
        case State::SynReceived:
            return "SynReceived";
        case State::Established:
            return "Established";
        case State::CloseWait:
            return "CloseWait";
        case State::LastAck:
            return "LastAck";
        case State::FinWait1:
            return "FinWait1";
        case State::FinWait2:
            return "FinWait2";
        case State::Closing:
            return "Closing";
        case State::TimeWait:
            return "TimeWait";
        default:
            return "None";
        }
    }

    enum class Error {
        None,
        FINDuringConnect,
        RSTDuringConnect,
        UnexpectedFlagsDuringConnect,
        RetransmitTimeout,
    };

    static const char* to_string(Error error)
    {
        switch (error) {
        case Error::None:
            return "None";
        case Error::FINDuringConnect:
            return "FINDuringConnect";
        case Error::RSTDuringConnect:
            return "RSTDuringConnect";
        case Error::UnexpectedFlagsDuringConnect:
            return "UnexpectedFlagsDuringConnect";
        default:
            return "Invalid";
        }
    }

    State state() const { return m_state; }
    void set_state(State state);

    Direction direction() const { return m_direction; }

    bool has_error() const { return m_error != Error::None; }
    Error error() const { return m_error; }
    void set_error(Error error) { m_error = error; }

    void set_ack_number(u32 n) { m_ack_number = n; }
    void set_sequence_number(u32 n) { m_sequence_number = n; }
    u32 ack_number() const { return m_ack_number; }
    u32 sequence_number() const { return m_sequence_number; }
    u32 packets_in() const { return m_packets_in; }
    u32 bytes_in() const { return m_bytes_in; }
    u32 packets_out() const { return m_packets_out; }
    u32 bytes_out() const { return m_bytes_out; }

    static constexpr u32 maximum_duplicate_acks = 5;
    void set_duplicate_acks(u32 acks) { m_duplicate_acks = acks; }
    u32 duplicate_acks() const { return m_duplicate_acks; }

    KResult send_ack(bool allow_duplicate = false);
    KResult send_tcp_packet(u16 flags, const UserOrKernelBuffer* = nullptr, size_t = 0, RoutingDecision* = nullptr);
    void receive_tcp_packet(const TCPPacket&, u16 size);

    bool should_delay_next_ack() const;

    static Lockable<HashMap<IPv4SocketTuple, TCPSocket*>>& sockets_by_tuple();
    static RefPtr<TCPSocket> from_tuple(const IPv4SocketTuple& tuple);

    static Lockable<HashMap<IPv4SocketTuple, RefPtr<TCPSocket>>>& closing_sockets();

};

}