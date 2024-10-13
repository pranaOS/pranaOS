/**
 * @file packettypes.h
 * @author Krisna Pranav
 * @brief Packet Type
 * @version 6.0
 * @date 2024-10-13
 * 
 * @copyright Copyright (c) 2021-2024, pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/types.h>

namespace Kernel::USB 
{

    static constexpr u8 BM_REQUEST_HOST_TO_DEVICE = (0 << 7);
    static constexpr u8 BM_REQUEST_DEVICE_TO_HOST = (1 << 7);
    static constexpr u8 BM_REQUEST_TYPE_STANDARD = (0 << 5);
    static constexpr u8 BM_REQUEST_TYPE_CLASS = (1 << 5);
    static constexpr u8 BM_REQUEST_TYPE_VENDOR = (2 << 5);
    static constexpr u8 BM_REQUEST_TYPE_RESERVED = (3 << 5);
    static constexpr u8 BM_REQUEST_RECIPEINT_DEVICE = (0 << 0);
    static constexpr u8 BM_REQUEST_RECIPIENT_INTERFACE = (1 << 0);
    static constexpr u8 BM_REQUEST_RECIPIENT_ENDPOINT = (2 << 0);
    static constexpr u8 BM_REQUEST_RECIPIENT_OTHER = (3 << 0);

    struct USBRequestData 
    {
        u8 request_type;
        u8 request;
        u16 value;
        u16 index;
        u16 length;
    }; // struct USBRequestData

} // namespace Kernel::USB