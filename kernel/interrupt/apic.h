/**
 * @file apic.h
 * @author Krisna Pranav
 * @brief apic
 * @version 1.0
 * @date 2023-08-17
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

#include <mods/types.h>
#include <kernel/time/hardwaretimer.h>
#include <kernel/vm/memorymanager.h>

namespace Kernel
{
    class APICTimer;

    struct LocalAPIC 
    {
        u32 apic_id;
    }; // struct

    class APIC
    {
    private:
        class ICRReg
        {

        }; 

    OwnPtr<Region> m_apic_base;
    Vector<OwnPtr<Processor>> m_ap_processor_info;
    Vector<Thread*> m_ap_idel_threads;

    static PhysicalAddress get_base();

    static void set_base(const PhysicalAddress& base);

    void write_register(u32 offset, u32 value);

    u32 read_register(u32 offset);

    void set_lvt(u32 offset, u8 interrupt);
        
    };

} // namespace Kernel