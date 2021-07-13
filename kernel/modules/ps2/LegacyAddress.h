/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

enum LegacyAddress
{
    LEGACY_MOUSE,
    LEGACY_KEYBOARD,

    LEGACY_ATA0,
    LEGACY_ATA1,
    LEGACY_ATA2,
    LEGACY_ATA3,

    LEGACY_COM1 = 0X3F8,
    LEGACY_COM2 = 0X2F8,
    LEGACY_COM3 = 0X3E8,
    LEGACY_COM4 = 0X2E8,
};