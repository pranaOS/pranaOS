/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#define DEVICE_PATH "/Devices"

#define KEYBOARD_DEVICE_PATH DEVICE_PATH "/keyboard"

#define MOUSE_DEVICE_PATH DEVICE_PATH "/mouse"

#define TEXTMODE_DEVICE_PATH DEVICE_PATH "/textmode"


#define UNIX_DEVICE_PATH(__device) DEVICE_PATH "/" __device