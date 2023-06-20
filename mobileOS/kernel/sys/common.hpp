/**
* @file common.hpp
* @author Krisna Pranav
* @brief Common
* @version 1.0
* @date 2023-06-20
*
* @copyright Copyright (c) 2021-2023, pranaOS Developers, Krisna Pranav
*
 */

#pragma once

#include "sys_return_codes.hpp"

namespace sys {

    enum class BusChannel {
        Unknown,
        System,
        SystemManagerRequests,
        PowerManagerRequests,
        USBNotifications,
        ServiceCellularNotifications,
        Test2CustomBusChannel,

    };

    enum class ServicePriority {
        Idle = 0,
        Low = 1,
        Normal = 2,
        High = 3,
        Realtime = 4,
    };

    enum class ServicePowerMode {
        Active,
        SuspendToRAM,
        SuspedToNVM,
    };

    inline const char *c_str(sys::ReturnCodes code) {
        switch (code) {
        case sys::ReturnCodes::Success:
            return "Success";
        case sys::ReturnCodes::Failure:
            return "Failure";
        case sys::ReturnCodes::Timeout:
            return "Timeout";
        case sys::ReturnCodes::ServicesDoesntExists:
            return "ServiceDoesntExists";
        case sys::ReturnCodes::Unresolved:
            return "Unresolved"
        }

        return "Undefined"
    }
}