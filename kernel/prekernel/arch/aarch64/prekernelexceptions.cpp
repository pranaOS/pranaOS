/**
 * @file prekernelexceptions.cpp
 * @author Krisna Pranav
 * @brief PreKernelExceptions
 * @version 6.0
 * @date 2024-10-24
 *
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 *
 */

#include <kernel/arch/aarch64/asm_wrapper.h>
#include <kernel/arch/aarch64/registers.h>
#include <kernel/prekernel/Arch/aarch64/aarch64_asm_utils.h>
#include <kernel/prekernel/Arch/aarch64/prekernel.h>

extern "C" void enter_el2_from_el3();
extern "C" void enter_el1_from_el2();

using namespace Kernel;

namespace Prekernel
{

    static void drop_to_el2()
    {
        Aarch64::SCR_EL3 secure_configuration_register_el3 = {};

        secure_configuration_register_el3.ST = 1;  
        secure_configuration_register_el3.RW = 1;  
        secure_configuration_register_el3.NS = 1;  
        secure_configuration_register_el3.HCE = 1; 

        Aarch64::SCR_EL3::write(secure_configuration_register_el3);

        Aarch64::SPSR_EL3 saved_program_status_register_el3 = {};

        saved_program_status_register_el3.A = 1;
        saved_program_status_register_el3.I = 1;
        saved_program_status_register_el3.F = 1;
        saved_program_status_register_el3.D = 1;
        saved_program_status_register_el3.M = Aarch64::SPSR_EL3::Mode::EL2t;
        Aarch64::SPSR_EL3::write(saved_program_status_register_el3);

        enter_el2_from_el3();
    }
    
    static void drop_to_el1()
    {
        Aarch64::HCR_EL2 hypervisor_configuration_register_el2 = {};
        hypervisor_configuration_register_el2.RW = 1; 
        Aarch64::HCR_EL2::write(hypervisor_configuration_register_el2);

        Aarch64::SPSR_EL2 saved_program_status_register_el2 = {};

        saved_program_status_register_el2.A = 1;
        saved_program_status_register_el2.I = 1;
        saved_program_status_register_el2.F = 1;
        saved_program_status_register_el2.M = Aarch64::SPSR_EL2::Mode::EL1t;

        Aarch64::SPSR_EL2::write(saved_program_status_register_el2);
        enter_el1_from_el2();
    }

    static void set_up_el1()
    {
        Aarch64::SCTLR_EL1 system_control_register_el1 = Aarch64::SCTLR_EL1::reset_value();

        system_control_register_el1.UCT = 1;   
        system_control_register_el1.nTWE = 1;  
        system_control_register_el1.nTWI = 1;  
        system_control_register_el1.DZE = 1;   
        system_control_register_el1.UMA = 1; 
        system_control_register_el1.SA0 = 1; 
        system_control_register_el1.SA = 1;  
        system_control_register_el1.A = 1;   

        Aarch64::SCTLR_EL1::write(system_control_register_el1);
    }

    /**
     * @brief drop to exception leve - 1
     * 
     */
    void drop_to_exception_level_1()
    {
        switch(Kernel::Aarch64::Asm::get_current_exception_level())
        {
        case Kernel::Aarch64::Asm::ExceptionLevel::EL3:
            drop_to_el2();
            [[fallthrough]];
        case Kernel::Aarch64::Asm::ExceptionLevel::EL2:
            drop_to_el1();
            [[fallthrough]];
        case Kernel::Aarch64::Asm::ExceptionLevel::EL1:
            set_up_el1();
            break;
        default:
        {
            Prekernel::panic("FATAL: CPU booted in unsupported exception mode!\r\n");
        }
        }
    }

} // namespace Prekernel
