/**
 * @file device_factory
 * @author Krisna Pranav
 * @brief device factory
 * @version 6.0
 * @date 2024-08-05
 *
 * @copyright Copyright (c) 2021 - 2024 pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include "disk.hpp"
#include <memory>

namespace purefs
{
    class DeviceFactory
    {
    public:
        virtual ~DeviceFactory() = default;
        virtual std::unique_ptr<blkdev::disk> makeDefaultBlockDevice() = 0;
    }; // class DeviceFactory
} // namespace purefs
