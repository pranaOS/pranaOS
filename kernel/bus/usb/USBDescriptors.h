/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Types.h>

namespace Kernel::USB {

struct [[gnu::packed]] USBDescriptorCommon {
    u8 length;
    u8 descriptor_type;
};

struct [[gnu::packed]] USBDeviceDescriptor {
    USBDescriptorCommon descriptor_header;
    u16 usb_spec_compliance_bcd;
    u8 device_class;
    u8 device_sub_class;
    u8 device_protocol;
    u8 max_packet_size;
    u16 vendor_id;
    u16 product_id;
    u16 device_release_bcd;
    u8 manufacturer_id_descriptor_index;
    u8 product_string_descriptor_index;
    u8 serial_number_descriptor_index;
    u8 num_configurations;
};

struct [[gnu::packed]] USBConfigurationDescriptor {
    USBDescriptorCommon descriptor_header;
    u16 total_length;
    u8 number_of_interfaces;
    u8 configuration_value;
    u8 configuration_string_descriptor_index;
    u8 attributes_bitmap;
    u8 max_power_in_ma;
};

struct [[gnu::packed]] USBInterfaceDescriptor {
    USBDescriptorCommon descriptor_header;
    u8 interface_id;
    u8 alternate_setting;
    u8 number_of_endpoints;
    u8 interface_class_code;
    u8 interface_sub_class_code;
    u8 interface_protocol;
    u8 interface_string_descriptor_index;
};

struct [[gnu::packed]] USBEndpointDescriptor {
    USBDescriptorCommon descriptor_header;
    u8 endpoint_address;
    u8 endpoint_attributes_bitmap;
    u16 max_packet_size;
    u8 poll_interval_in_frames;
};

static constexpr u8 DESCRIPTOR_TYPE_DEVICE = 0x01;
static constexpr u8 DESCRIPTOR_TYPE_CONFIGURATION = 0x02;
static constexpr u8 DESCRIPTOR_TYPE_STRING = 0x03;
static constexpr u8 DESCRIPTOR_TYPE_INTERFACE = 0x04;
static constexpr u8 DESCRIPTOR_TYPE_ENDPOINT = 0x05;
static constexpr u8 DESCRIPTOR_TYPE_DEVICE_QUALIFIER = 0x06;

}
