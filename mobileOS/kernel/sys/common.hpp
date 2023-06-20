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

    /**
     * @brief BusChannel
     * 
     */
    enum class BusChannel {
        Unknown,
        System,
        SystemManagerRequests,
        PowerManagerRequests,
        USBNotifications,
        ServiceCellularNotifications,
        Test2CustomBusChannel,
        ServiceDBNotifications,
        ServiceAudioNotifications,
        AppManagerNotifications,
        ServiceFotaNotifications,
        AntennaNotifications,
        ServiceEvtmgrNotifications,
        PhoneModeChanges,
        PhoneLockChanges,
        BluetoothModeChanges,
        BluetoothNotifications
    };

    /**
     * @brief ServicePriority
     * 
     */
    enum class ServicePriority {
        Idle     = 0,
        Low      = 1,
        Normal   = 2,
        High     = 3,
        Realtime = 4,
    };

    /**
     * @brief ServicePowerMode
     * 
     */
    enum class ServicePowerMode {
        Active,
        SuspendToRAM,
        SuspendToNVM
    };

    /**
     * @brief CloseReason
     * 
     */
    enum class CloseReason {
        RegularPowerDown,
        OnboardingPowerDown,
        Reboot,
        RebootToRecovery,
        FactoryReset,
        SystemBrownout,
        LowBattery
    };

    /**
     * @brief RecoveryReason
     * 
     */
    enum class RecoveryReason {
        Update,
        Recovery,
        FactoryReset,
        Backup,
        Restore
    };

}

/**
 * @brief c_str
 * 
 * @param code 
 * @return const char* 
 */
inline const char *c_str(sys::ReturnCodes code) {
    switch (code) {
    case sys::ReturnCodes::Success:
        return "Success";

    case sys::ReturnCodes::Failure:
        return "Failure";

    case sys::ReturnCodes::Timeout:
        return "Timeout";

    case sys::ReturnCodes::ServiceDoesntExist:
        return "ServiceDoesntExist";

    case sys::ReturnCodes::Unresolved:
        return "Unresolved";
    }
    
    return "Undefined";
}

/**
 * @brief c_str
 * 
 * @param code 
 * @return const char* 
 */
inline const char *c_str(sys::ServicePowerMode code) {
    switch (code) {
    case sys::ServicePowerMode::Active:
        return "Active";

    case sys::ServicePowerMode::SuspendToRAM:
        return "SuspendToRAM";

    case sys::ServicePowerMode::SuspendToNVM:
        return "SuspendToNVM";
    }
    return "";
}

/**
 * @brief c_str
 * 
 * @param channel 
 * @return const char* 
 */
inline const char *c_str(sys::BusChannel channel) {
    switch (channel) {
    case sys::BusChannel::Unknown:
        return "Unknown";

    case sys::BusChannel::System:
        return "System";

    case sys::BusChannel::SystemManagerRequests:
        return "SystemManagerRequests";

    case sys::BusChannel::PowerManagerRequests:
        return "PowerManagerRequests";

    case sys::BusChannel::USBNotifications:
        return "USBNotifications";

    case sys::BusChannel::ServiceCellularNotifications:
        return "ServiceCellularNotifications";

    case sys::BusChannel::Test2CustomBusChannel:
        return "Test2CustomBusChannel";

    case sys::BusChannel::ServiceDBNotifications:
        return "ServiceDBNotifications";

    case sys::BusChannel::ServiceAudioNotifications:
        return "ServiceAudioNotifications";

    case sys::BusChannel::AppManagerNotifications:
        return "AppManagerNotifications";

    case sys::BusChannel::ServiceFotaNotifications:
        return "ServiceFotaNotifications";

    case sys::BusChannel::AntennaNotifications:
        return "AntennaNotifications";

    case sys::BusChannel::ServiceEvtmgrNotifications:
        return "ServiceEvtmgrNotifications";

    case sys::BusChannel::PhoneModeChanges:
        return "PhoneModeChanges";

    case sys::BusChannel::BluetoothModeChanges:
        return "BluetoothModeChanges";

    case sys::BusChannel::BluetoothNotifications:
        return "BluetoothNotifications";

    case sys::BusChannel::PhoneLockChanges:
        return "PhoneLockChanges";

    }
    return "";
}
