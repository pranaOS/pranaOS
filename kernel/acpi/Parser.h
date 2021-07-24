/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <base/Types.h>
#include <kernel/acpi/Definitions.h>
#include <kernel/acpi/Initialize.h>
#include <kernel/FileSystem/SysFSComponent.h>
#include <kernel/PhysicalAddress.h>
#include <kernel/vm/Region.h>
#include <kernel/VirtualAddress.h>

namespace Kernel::ACPI {

class ACPISysFSDirectory : public SysFSDirectory {
public:
    static void initialize();

private:
    ACPISysFSDirectory();
};

class ACPISysFSComponent : public SysFScomponent {
public:
    static NonnullRefPtr<ACPISysFSComponent> create(String name, PhysicalAddress, size_t table_size);

    virtual KResultOr<size_t> read_bytes(off_t, size_t, UserOrKernelBuffer&, FileDescription*) const override;

protected:
    OwnPtr<KBuffer> try_to_generate() const;
    ACPISysFSComponent(String name, PhysicalAddress, size_t table_size);

    PhysicalAddress m_paddr;
    size_t m_length;

};

}