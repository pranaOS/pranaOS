/**
 * @file validation.h
 * @author Krisna Pranav
 * @brief validation
 * @version 6.0
 * @date 2025-03-09
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <libdevicetree/flatteneddevicetree.h>

namespace DeviceTree 
{

    enum class Verbose {
        No,
        Yes
    }; // enum class Verbose

    /**
     * @param header 
     * @param blob_start 
     * @param blob_size 
     * @return true 
     * @return false 
     */
    bool validate_flattened_device_tree(FlattenedDeviceTreeHeader const& header, u8 const* blob_start, size_t blob_size, Verbose = Verbose::No);

    /**
     * @param header 
     * @param blob_start 
     * @param blob_size 
     * @return true 
     * @return false 
     */
    bool dump(FlattenedDeviceTreeHeader const& header, u8 const* blob_start, size_t blob_size);

} // namespace DeviceTree 
