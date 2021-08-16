/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <kernel/KResult.h>
#include <kernel/locking/ProtectedValue.h>
#include <kernel/net/IPv4Socket.h>

namespace Kernel {

class UDPSocket final : public IPv4Socket {
public:
    static KResultOr<NonnullRefPtr<UDPSocket>> create(int protocol, NonnullOwnPtr<DoubleBuffer> receive_buffer);
    virtual ~UDPSocket() override;

    static SocketHandle<UDPSocket> from_port(u16);
    static void for_each(Function<void(const UDPSocket&)>);

private:
    explicit UDPSocket(int protocol, NonnullOwnPtr<DoubleBuffer> receive_buffer);
    virtual StringView class_name() const override { return "UDPSocket"; }
    static ProtectedValue<HashMap<u16, UDPSocket*>>& sockets_by_port();

    virtual KResultOr<size_t> protocol_receive(ReadonlyBytes raw_ipv4_packet, UserOrKernelBuffer& buffer, size_t buffer_size, int flags) override;
    virtual KResultOr<size_t> protocol_send(const UserOrKernelBuffer&, size_t) override;
    virtual KResult protocol_connect(FileDescription&, ShouldBlock) override;
    virtual KResultOr<u16> protocol_allocate_local_port() override;
    virtual KResult protocol_bind() override;
};

}
