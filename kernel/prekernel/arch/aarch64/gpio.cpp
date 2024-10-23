/**
 * @file gpio.cpp
 * @author Krisna Pranav
 * @brief GPIO
 * @version 6.0
 * @date 2024-10-23
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/prekernel/arch/aarch64/gpio.h>
#include <kernel/prekernel/arch/aarch64/mmio.h>

/**
 * @return n
 * 
 */
extern "C" void wait_cycles(int n);

namespace Prekernel 
{

    struct PinData 
    {
        u32 bits[2];
        u32 reserved;
    }; // struct PinData

    struct GPIOControlRegisters 
    {
        u32 function_select[6]; 
        u32 reserved;
        PinData output_set;
        PinData output_clear;
        PinData level;
        PinData event_detect_status;
        PinData rising_edge_detect_enable;
        PinData falling_edge_detect_enable;
        PinData high_detect_enable;
        PinData low_detect_enable;
        PinData async_rising_edge_detect_enable;
        PinData async_falling_edge_detect_enable;
        u32 pull_up_down_enable;
        PinData pull_up_down_enable_clock;
        u32 test;
    }; // strcut GPIOControlRegisters

    /**
     * @brief Construct a new GPIO::GPIO object
     * 
     */
    GPIO::GPIO()
        : m_registers(MMIO::the().peripheral<GPIOControlRegisters>(0x20'0000))
    {
    }

    /**
     * @return GPIO& 
     */
    GPIO& GPIO::the()
    {
        static GPIO instance;
        return instance;
    }

    /**
     * @param pin_number 
     * @param function 
     */
    void GPIO::set_pin_function(unsigned pin_number, PinFunction function)
    {
        unsigned function_select_index = pin_number / 10;
        unsigned function_select_bits_start = (pin_number % 10) * 3;

        u32 function_bits = m_registers->function_select[function_select_index];
        function_bits = (function_bits & ~(0b111 << function_select_bits_start)) | (static_cast<u32>(function) << function_select_bits_start);
        m_registers->function_select[function_select_index] = function_bits;
    }

    /**
     * @param enable 
     * @param state 
     */
    void GPIO::internal_enable_pins(u32 enable[2], PullUpDownState state)
    {
        m_registers->pull_up_down_enable = static_cast<u32>(state);

        wait_cycles(150);

        m_registers->pull_up_down_enable_clock.bits[0] = enable[0];
        m_registers->pull_up_down_enable_clock.bits[1] = enable[1];

        wait_cycles(150);

        m_registers->pull_up_down_enable = 0;
        m_registers->pull_up_down_enable_clock.bits[0] = 0;
        m_registers->pull_up_down_enable_clock.bits[1] = 0;

    }
} // namespace Prekernel
