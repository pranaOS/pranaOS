/**
 * @file messagetype.hpp
 * @author Krisna Pranav
 * @brief Message Types
 * @version 6.0
 * @date 2024-08-09
 *
 * @copyright Copyright (c) 2024 Krisna Pranav, pranaOS Developers
 *
 */

#pragma ocne

enum class MessageType
{
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
    DBContactGetBySpeedDialg [[depricated]]
}; // enum class MessageType