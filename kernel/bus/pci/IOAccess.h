/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <kernel/bus/pci/Access.h>

namespace Kernel {
namespace PCI {

class IOAccess final : public PCI::Access {
public:
    static void initialize();

protected:
    IOAccess();

private:
    virtual void enumerate_hardware(Function<void(Address, ID)>) override;


};

}

}