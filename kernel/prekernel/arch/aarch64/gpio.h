/**
 * @file gpio.h
 * @author Krisna Pranav
 * @brief GPIO
 * @version 6.0
 * @date 2024-10-23
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/array.h>
#include <mods/types.h>

namespace Prekernel 
{
    struct GPIOControlRegisters;

    class GPIO 
    {
    public:
        enum class PinFunction 
        {
            Input = 0,
            Output = 1,
            Alternate0 = 0b100,
            Alternate1 = 0b101,
            Alternate2 = 0b110,
            Alternate3 = 0b111,
            Alternate4 = 0b011,
            Alternate5 = 0b010,
        }; // enum class PinFunction

        static GPIO& the();

        /**
         * @brief Set the pin function object
         * 
         * @param pin_number 
         */
        void set_pin_function(unsigned pin_number, PinFunction);

        enum class PullUpDownState 
        {
            Disable = 0,
            PullDown = 1,
            PullUp = 2,
        }; // enum class PullUpDownState

        /**
         * @brief Set the pin pull up down state object
         * 
         * @tparam N 
         * @param pins 
         * @param state 
         */
        template<size_t N>
        void set_pin_pull_up_down_state(Array<int, N> pins, PullUpDownState state)
        {
            u32 enable[2] = {};
            for (int pin : pins) {
                if (pin < 32)
                    enable[0] |= (1 << pin);
                else
                    enable[1] |= (1 << (pin - 32));
            }
            internal_enable_pins(enable, state);
        }

    private:

        /**
         * @brief Construct a new GPIO object
         * 
         */
        GPIO();

        /**
         * @param enable 
         * @param state 
         */
        void internal_enable_pins(u32 enable[2], PullUpDownState state);

        GPIOControlRegisters volatile* m_registers;
    }; // class GPIO
} // namespace Prekernel