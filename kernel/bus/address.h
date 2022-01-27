/*
 * Copyright (c) 2021 - 2022, the pranaOS Developers & Krisna Pranav
 *
 * SPDX-License-Identifier: GPL-3.0 License
 */

#pragma once

namespace Kernel {
    struct Address {
        Address() = default;
        Address(const Address& address) = default;
    };

    class busAddress {
      public:
        busAddress();
        Address address_t;
    };
}