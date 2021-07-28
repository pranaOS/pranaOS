/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Types.h>


static constexpr u8 USB_DEVICE_REQUEST_DEVICE_TO_HOST = 0x80;
static constexpr u8 USB_DEVICE_REQUEST_HOST_TO_DEVICE = 0x00;
static constexpr u8 USB_INTERFACE_REQUEST_DEVICE_TO_HOST = 0x81;
static constexpr u8 USB_INTERFACE_REQUEST_HOST_TO_DEVICE = 0x01;
static constexpr u8 USB_ENDPOINT_REQUEST_DEVICE_TO_HOST = 0x82;
static constexpr u8 USB_ENDPOINT_REQUEST_HOST_TO_DEVICE = 0x02;

static constexpr u8 USB_REQUEST_GET_STATUS = 0x00;
static constexpr u8 USB_REQUEST_CLEAR_FEATURE = 0x01;
static constexpr u8 USB_REQUEST_SET_FEATURE = 0x03;
static constexpr u8 USB_REQUEST_SET_ADDRESS = 0x05;
static constexpr u8 USB_REQUEST_GET_DESCRIPTOR = 0x06;
static constexpr u8 USB_REQUEST_SET_DESCRIPTOR = 0x07;
static constexpr u8 USB_REQUEST_GET_CONFIGURATION = 0x08;
static constexpr u8 USB_REQUEST_SET_CONFIGURATION = 0x09;