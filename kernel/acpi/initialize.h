/**
 * @file initialize.h
 * @author Krisna Pranav
 * @brief initialize
 * @version 1.
 * @date 2023-08-14
 * 
 * @copyright Copyright (c) 2021-2024 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once 

namespace Kernel 
{
    
    namespace ACPI 
    {
        bool is_enabled();
        void initialize();
    } // namespace ACPI

} // namespace Kernel
