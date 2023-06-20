/**
* @file message_type.hpp
* @author Krisna Pranav
* @brief Message Type
* @version 1.0
* @date 2023-06-20
*
* @copyright Copyright (c) 2021-2023, pranaOS Developers, Krisna Pranav
*
*/

#pragma once

/**
 * @brief MessageType
 */
enum class MessageType {
    MessageTypeUninitialized = 0,
    EinkMessage,
    GUIMessage,
    GUIFocusInfo,
    DBServiceEvents,
    DBServiceNotification,
    DBSyncPackage,
    DBSettingsGet,
    DBSettingsUpdate,
    DBContactGetByID [[deprecated]],
    DBContactGetBySpeedDial [[deprecated]],
    DBContactMatchByNumberID [[deprecated]],
    DBContactMatchByNumber
    [[deprecated]],
    DBMatchContactNumberBesidesOfContactID
    [[deprecated]],

    DBContactAdd [[deprecated]],
    DBContactRemove [[deprecated]],
    DBContactUpdate [[deprecated]],

    DBQuery,

    DBCalllogAdd [[deprecated]],
    DBCalllogRemove [[deprecated]],
    DBCalllogUpdate [[deprecated]],

    AudioMessage,

    APMGeneric,
    APMAction,
    APMFinish,
    APMCheckAppRunning,
    APMSwitch,
    APMSwitchPrevApp,
    APMConfirmSwitch,

    APMConfirmClose,
    APMConfirmWindowSwitch,
    APMFinalizingClose,
    APMRegister,

    APMInit,
    APMDelayedClose,

    APMChangeLanguage,

    APMClose,
    APMPreventBlocking,
    APMInitPowerSaveMode,


    KBDKeyEvent,

    AppMessage,
    AppAction,
    AppSwitch,
    AppSwitchWindow,
    AppInputEvent,
    AppRefresh,
    AppRebuild,
    AppClose,
    AppFocus,
    AppFocusLost,
    AppSwitchBack,
    EVMFocusApplication,
    EVMKeyboardProfile,
    PMChangePowerMode,
    DeviceRegistration,
    SystemManagerCpuFrequency,
    SystemManagerSentinelRegistration,
    SystemManagerSentinelRemoval,
    EVMMinuteUpdated,
    EVMTimeUpdated,
    EVMModemStatus,
    EVMRingIndicator,
    BluetoothRequest,
    BluetoothScanResult,
    BluetoothAddrResult,
    BluetoothPairResult,
    BluetoothAudioStart,
    LwIP_request,
    EVM_GPIO,
    SIMTrayEvent,
    UpdateOS,
    Backup,
    Sync,
    Restore,
    Factory,
    DeveloperModeRequest,
    DeveloperModeMessageWrapper,
    PasscodeRequest,
    TransferTimer,
    USBConnected,
    USBConfigured,
    USBDisconnected,
    HttpRequest,
    HttpResponse,
    FotaConfigureAPN,
    FotaInternetConnect,
    FotaInternetDisconnect,
    FotaInternetNotification,
    FotaStart,
    FotaProgress,
    FotaFinished,
    StateChange,
    AntennaChanged,
    AntennaCSQChange,
    AntennaLockService,
    AntennaGetLockState,
    AntennaLockNotification,
    Settings,
    FileContentModified,
    FileIndexer,
    ScreenLightControlAction,
    ScreenLightControlParameters,
    ScreenLightControlParametersResponse,
    VibratorPulseMessage,
    VibratorLevelMessage,
    Quotes,
    AlarmMessage,
    LatchStateMessage
};