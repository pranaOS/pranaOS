/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <kernel/net/NetworkAdapter.h>
#include <kernel/Thread.h>

namespace Kernel {

struct RoutingDecision {
    RefPtr<NetworkAdapter> adapter;
    MACAddress next_hop;

    bool is_zero() const;
};

enum class UpdateArp {
    Set,
    Delete,
};

void update_arp_table(const IPv4Address&, const MACAddress&, UpdateArp update);
RoutingDecision route_to(const IPv4Address& target, const IPv4Address& source, const RefPtr<NetworkAdapter> through = nullptr);

Lockable<HashMap<IPv4Address, MACAddress>>& arp_table();

}