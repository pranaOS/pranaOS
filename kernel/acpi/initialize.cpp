/**
 * @file initialize.cpp
 * @author Krisna Pranav
 * @brief initialize
 * @version 1.0
 * @date 2023-08-17
 * 
 * @copyright Copyright (c) 2021 - 2023 pranaOS Developers, Krisna Pranav
 * 
 */

#include <kernel/commandline.h>
#include <kernel/acpi/dynamicparser.h>

namespace Kernel
{
    namespace ACPI
    {
        enum class FeatureLevel
        {
            Enabled,
            Limited,
            Disabled,
        }; // enum

        /**
         * @return FeatureLevel 
         */
        static FeatureLevel determine_feature_level()
        {
            auto value = kernel_command_line().lookup("acpi").value_or("on");

            if (value == "limited")
                return FeatureLevel::Limited;
            if (value == "off")
                return FeatureLevel::Disabled;
            
            return FeatureLevel::Enabled;
        }

        bool is_enabled()
        {
            return Parser::the();
        }
    }
}