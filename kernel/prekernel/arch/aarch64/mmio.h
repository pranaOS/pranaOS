/**
 * @file mmio.h
 * @author Krisna Pranav
 * @brief MMIO
 * @version 6.0
 * @date 2024-10-23
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/types.h>

namespace Prekernel 
{
    class MMIO 
    {
    public:

        /**
         * @return MMIO& 
         */
        static MMIO& the();

        /**
         * @param offset 
         * @return u32 
         */
        u32 read(FlatPtr offset) 
        { 
            return *peripheral_address(offset); 
        }

        /**
         * @param offset 
         * @param value 
         */
        void write(FlatPtr offset, u32 value) 
        { 
            *peripheral_address(offset) = value; 
        }

        /**
         * @param offset 
         * @return u32* 
         */
        u32 volatile* peripheral_address(FlatPtr offset) 
        { 
            return (u32 volatile*)(m_base_address + offset); 
        }

        /**
         * @tparam T 
         * @param offset 
         * @return T* 
         */
        template<class T>
        T volatile* peripheral(FlatPtr offset) 
        { 
            return (T volatile*)peripheral_address(offset); 
        }

        /**
         * @return FlatPtr 
         */
        FlatPtr peripheral_base_address() const 
        { 
            return m_base_address; 
        }

        /**
         * @return FlatPtr 
         */
        FlatPtr peripheral_end_address() const 
        { 
            return m_base_address + 0x00FFFFFF; 
        }

    private:
        /**
         * @brief Construct a new MMIO object
         * 
         */
        MMIO();

        unsigned int m_base_address;
    }; // class MMIO
} // namespace Prekernel