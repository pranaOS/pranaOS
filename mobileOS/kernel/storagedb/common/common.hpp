/**
 * @file common.hpp
 * @author Krisna Pranav
 * @brief Common
 * @version 6.0
 * @date 2023-06-17
 *
 * @copyright Copyright (c) 2021-2024, pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include <cstdint>

/**
 * @brief SMSType
 *
 */
enum class SMSType : uint32_t
{
    DRAFT = 0x01,
    FAILED = 0x02,
    INBOX = 0x04,
    OUTBOX = 0x08,
    QUEUED = 0x10,
    INPUT = 0x12,
    UNKNOWN = 0xFF
};

/**
 * @brief EntryState
 *
 */
enum class EntryState
{
    ALL,
    READ,
    UNREAD
};

/**
 * @brief ContactNumberType
 *
 */
enum class ContactNumberType
{
    CELL = 0,
    HOME,
    WORK,
    FAKS,
    PAGER,
    OTHER
};

/**
 * @brief ContactAddressType
 *
 */
enum class ContactAddressType
{
    HOME = 0,
    WORK,
    OTHER
};

/**
 * @brief SettingsLanguage
 *
 */
enum class SettingsLanguage
{
    ENGLISH = 0x01,
    POLISH = 0x02,
    GERMAN = 0x04,
    SPANISH = 0x08,
};

/**
 * @brief SettingsPinMode
 *
 */
enum class SettingsPinMode
{
    ALWAYS = 0,
    DAYS = 1
};