
/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <base/OwnPtr.h>
#include <base/Singleton.h>
#include <kernel/API/MousePacket.h>
#include <kernel/Arch/x86/InterruptDisabler.h>
#include <kernel/CommandLine.h>
#include <kernel/Debug.h>
#include <kernel/Devices/VMWareBackdoor.h>
#include <kernel/Sections.h>

namespace Kernel {

#define VMWARE_CMD_GETVERSION 0x0a

#define VMMOUSE_READ_ID 0x45414552
#define VMMOUSE_DISABLE 0x000000f5
#define VMMOUSE_REQUEST_RELATIVE 0x4c455252
#define VMMOUSE_REQUEST_ABSOLUTE 0x53424152

#define VMMOUSE_QEMU_VERSION 0x3442554a
#define VMMOUSE_LEFT_CLICK 0x20
#define VMMOUSE_RIGHT_CLICK 0x10
#define VMMOUSE_MIDDLE_CLICK 0x08

#define VMWARE_MAGIC 0x564D5868
#define VMWARE_PORT 0x5658
#define VMWARE_PORT_HIGHBANDWIDTH 0x5659

inline void vmware_out(VMWareCommand& command)
{
    command.magic = VMWARE_MAGIC;
    command.port = VMWARE_PORT;
    command.si = 0;
    command.di = 0;
    asm volatile("in %%dx, %0"
                 : "+a"(command.ax), "+b"(command.bx), "+c"(command.cx), "+d"(command.dx), "+S"(command.si), "+D"(command.di));
}

inline void vmware_high_bandwidth_send(VMWareCommand& command)
{
    command.magic = VMWARE_MAGIC;
    command.port = VMWARE_PORT_HIGHBANDWIDTH;

    asm volatile("cld; rep; outsb"
                 : "+a"(command.ax), "+b"(command.bx), "+c"(command.cx), "+d"(command.dx), "+S"(command.si), "+D"(command.di));
}

inline void vmware_high_bandwidth_get(VMWareCommand& command)
{
    command.magic = VMWARE_MAGIC;
    command.port = VMWARE_PORT_HIGHBANDWIDTH;
    asm volatile("cld; rep; insb"
                 : "+a"(command.ax), "+b"(command.bx), "+c"(command.cx), "+d"(command.dx), "+S"(command.si), "+D"(command.di));
}

class VMWareBackdoorDetector {
public:
    VMWareBackdoorDetector()
    {
        if (detect_presence())
            m_backdoor = make<VMWareBackdoor>();
    }

    VMWareBackdoor* get_instance()
    {
        return m_backdoor.ptr();
    }

private:
    static bool detect_presence()
    {
        VMWareCommand command;
        command.bx = ~VMWARE_MAGIC;
        command.command = VMWARE_CMD_GETVERSION;
        vmware_out(command);
        if (command.bx != VMWARE_MAGIC || command.ax == 0xFFFFFFFF)
            return false;
        return true;
    }

    OwnPtr<VMWareBackdoor> m_backdoor;
};

}