/**
 * @file device_factory
 * @author Krisna Pranav
 * @brief device factory
 * @version 6.0
 * @date 2024-08-05
 *
 * @copyright Copyright (c) 2021 - 2025 pranaOS Developers, Krisna Pranav
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
        /**
         * @brief Destroy the Device Factory object
         *
         */
        virtual ~DeviceFactory() = default;

        /**
         * @return std::unique_ptr<blkdev::disk>
         */
        virtual std::unique_ptr<blkdev::disk> makeDefaultBlockDevice() = 0;

        /**
         * @return std::unique_ptr<blkdev::disk>
         */
        virtual std::unique_ptr<blkdev::disk> makeDefaultNvmDevice() = 0;
    }; // class DeviceFactory
} // namespace purefs
