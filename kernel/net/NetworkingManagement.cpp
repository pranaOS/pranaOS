/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/Singleton.h>
#include <kernel/CommandLine.h>
#include <kernel/IO.h>
#include <kernel/memory/AnonymousVMObject.h>
#include <kernel/Multiboot.h>
#include <kernel/net/E1000ENetworkAdapter.h>
#include <kernel/net/E1000NetworkAdapter.h>
#include <kernel/net/LoopbackAdapter.h>
#include <kernel/net/NE2000NetworkAdapter.h>
#include <kernel/net/NetworkingManagement.h>
#include <kernel/net/RTL8139NetworkAdapter.h>
#include <kernel/net/RTL8168NetworkAdapter.h>
#include <kernel/Sections.h>

namespace Kernel {

static Singleton<NetworkingManagement> s_the;

NetworkingManagement& NetworkingManagement::the()
{
    return *s_the;
}

bool NetworkingManagement::is_initialized()
{
    return s_the.is_initialized();
}

UNMAP_AFTER_INIT NetworkingManagement::NetworkingManagement()
{
}

NonnullRefPtr<NetworkAdapter> NetworkingManagement::loopback_adapter() const
{
    return *m_loopback_adapter;
}

void NetworkingManagement::for_each(Function<void(NetworkAdapter&)> callback)
{
    MutexLocker locker(m_lock);
    for (auto& it : m_adapters)
        callback(it);
}

RefPtr<NetworkAdapter> NetworkingManagement::from_ipv4_address(const IPv4Address& address) const
{
    MutexLocker locker(m_lock);
    for (auto& adapter : m_adapters) {
        if (adapter.ipv4_address() == address || adapter.ipv4_broadcast() == address)
            return adapter;
    }
    if (address[0] == 0 && address[1] == 0 && address[2] == 0 && address[3] == 0)
        return m_loopback_adapter;
    if (address[0] == 127)
        return m_loopback_adapter;
    return {};
}
RefPtr<NetworkAdapter> NetworkingManagement::lookup_by_name(const StringView& name) const
{
    MutexLocker locker(m_lock);
    RefPtr<NetworkAdapter> found_adapter;
    for (auto& it : m_adapters) {
        if (it.name() == name)
            found_adapter = it;
    }
    return found_adapter;
}

UNMAP_AFTER_INIT RefPtr<NetworkAdapter> NetworkingManagement::determine_network_device(PCI::Address address) const
{
    if (auto candidate = E1000NetworkAdapter::try_to_initialize(address); !candidate.is_null())
        return candidate;
    if (auto candidate = E1000ENetworkAdapter::try_to_initialize(address); !candidate.is_null())
        return candidate;
    if (auto candidate = RTL8139NetworkAdapter::try_to_initialize(address); !candidate.is_null())
        return candidate;
    if (auto candidate = RTL8168NetworkAdapter::try_to_initialize(address); !candidate.is_null())
        return candidate;
    if (auto candidate = NE2000NetworkAdapter::try_to_initialize(address); !candidate.is_null())
        return candidate;
    return {};
}

bool NetworkingManagement::initialize()
{
    if (!kernel_command_line().is_physical_networking_disabled()) {
        PCI::enumerate([&](const PCI::Address& address, PCI::ID) {

            if (PCI::get_class(address) != 0x02)
                return;
            if (auto adapter = determine_network_device(address); !adapter.is_null())
                m_adapters.append(adapter.release_nonnull());
        });
    }
    auto loopback = LoopbackAdapter::try_create();
    VERIFY(loopback);
    m_adapters.append(*loopback);
    m_loopback_adapter = loopback;
    return true;
}

}
