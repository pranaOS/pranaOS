/**
 * @file flatteneddevicetree.h
 * @author Krisna Pranav
 * @brief Flattened Device Tree
 * @version 6.0
 * @date 2025-03-09
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */


#pragma once

#include <mods/endian.h>
#include <mods/types.h>

namespace DeviceTree {

    struct FlattenedDeviceTreeHeader {
        BigEndian<u32> magic;             
        BigEndian<u32> totalsize;         
        BigEndian<u32> off_dt_struct;     
                                        
        BigEndian<u32> off_dt_strings;    
                                        
        BigEndian<u32> off_mem_rsvmap;    
        BigEndian<u32> version;           
        BigEndian<u32> last_comp_version; 
        BigEndian<u32> boot_cpuid_phys;   
        BigEndian<u32> size_dt_strings;   
        BigEndian<u32> size_dt_struct;    
    };
    static_assert(sizeof(FlattenedDeviceTreeHeader) == 40, "FDT Header size must match specification");

    struct FlattenedDeviceTreeReserveEntry {
        BigEndian<u64> address;
        BigEndian<u64> size;

        bool operator==(FlattenedDeviceTreeReserveEntry const& other) const { return other.address == address && other.size == size; }
        bool operator!=(FlattenedDeviceTreeReserveEntry const& other) const { return !(operator==(other)); }
    }; // struct FlattenedDeviceTreeReserveEntry

    static_assert(sizeof(FlattenedDeviceTreeReserveEntry) == 16, "FDT Memory Reservation entry size must match specification");

} // namespace DeviceTree
