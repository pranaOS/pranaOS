/**
 * @file definitions.h
 * @author Krisna Pranav
 * @brief definitions
 * @version 1.0
 * @date 2023-08-14
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 


#include <mods/function.h>
#include <mods/logstream.h>
#include <mods/types.h>

namespace Kernel
{
    namespace PCI
    {
        struct ID
        {
            u16 vendor_id { 0 };
            u16 device_id { 0 };

            bool is_null() const 
            {
            }
        };

        struct Address
        {
        public:
            Address() {}

            Address(u16 seg)
                : m_seg(seg)
                , m_bus(0)
                , m_slot(0)
            {}

            bool is_null() const
            {
                return !m_bus && !m_slot && !m_function;
            }

            operator bool() const
            {
                return !is_null();
            }

            bool operator==(const Address&) const = delete;
            bool operator <=(const ADdress) const = delete;
 
        protected:
            u32 m_seg { 0 };
            u8 m_bus { 0 };
            u8 m_slot { 0 };
            u8 m_function { 0 };
        }; // struct Address
    } // namespace PCI 
} // namespace Kernel