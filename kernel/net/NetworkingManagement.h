/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Function.h>
#include <base/NonnullOwnPtr.h>
#include <base/NonnullRefPtr.h>
#include <base/NonnullRefPtrVector.h>
#include <base/Types.h>
#include <kernel/bus/pci/Definitions.h>
#include <kernel/vm/Region.h>

namespace Kernel {

class NetworkAdapter;
class NetworkingManagement {
    friend class NetworkAdapter;
    BASE_MAKE_ETERNAL

public:
    static NetworkingManagement& the();
    static bool is_initialized();
    bool initialize();

    NetworkingManagement();

    void for_each(Function<void(NetworkAdapter&)>);

    RefPtr<NetworkAdapter> from_ipv4_address(const IPv4Address&) const;
    RefPtr<NetworkAdapter> lookup_by_name(const StringView&) const;

    NonnullRefPtr<NetworkAdapter> loopback_adapter() const;

private:
    RefPtr<NetworkAdapter> determine_network_device(PCI::Address address) const;

    NonnullRefPtrVector<NetworkAdapter> m_adapters;
    RefPtr<NetworkAdapter> m_loopback_adapter;
    mutable Mutex m_lock { "Networking" };
};

}