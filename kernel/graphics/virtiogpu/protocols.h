/**
 * @file protocols.h
 * @author Krisna Pranav
 * @brief Protocols 
 * @version 6.0
 * @date 2024-10-12
 * 
 * @copyright Copyright (c) 2021-2024, pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

#include <mods/types.h>

#define VIRTIO_GPU_MAX_SCANOUTS 16

namespace Kernel::Graphics::VirtIOGPU 
{
    TYPEDEF_DISTINCT_ORDERED_ID(u32, ResourceID);
    TYPEDEF_DISTINCT_ORDERED_ID(u32, ScanoutID);
}; // namespace Kernel::Graphics::VirtIOGPU

namespace Kernel::Graphics::VirtIOGPU::Protocol 
{

    enum class CommandType : u32 
    {
        VIRTIO_GPU_CMD_GET_DISPLAY_INFO = 0x0100,
        VIRTIO_GPU_CMD_RESOURCE_CREATE_2D,
        VIRTIO_GPU_CMD_RESOURCE_UNREF,
        VIRTIO_GPU_CMD_SET_SCANOUT,
        VIRTIO_GPU_CMD_RESOURCE_FLUSH,
        VIRTIO_GPU_CMD_TRANSFER_TO_HOST_2D,
        VIRTIO_GPU_CMD_RESOURCE_ATTACH_BACKING,
        VIRTIO_GPU_CMD_RESOURCE_DETACH_BACKING,
        VIRTIO_GPU_CMD_GET_CAPSET_INFO,
        VIRTIO_GPU_CMD_GET_CAPSET,
        VIRTIO_GPU_CMD_GET_EDID,
        VIRTIO_GPU_CMD_UPDATE_CURSOR = 0x0300,
        VIRTIO_GPU_CMD_MOVE_CURSOR,
        VIRTIO_GPU_RESP_OK_NODATA = 0x1100,
        VIRTIO_GPU_RESP_OK_DISPLAY_INFO,
        VIRTIO_GPU_RESP_OK_CAPSET_INFO,
        VIRTIO_GPU_RESP_OK_CAPSET,
        VIRTIO_GPU_RESP_OK_EDID,
        VIRTIO_GPU_RESP_ERR_UNSPEC = 0x1200,
        VIRTIO_GPU_RESP_ERR_OUT_OF_MEMORY,
        VIRTIO_GPU_RESP_ERR_INVALID_SCANOUT_ID,
        VIRTIO_GPU_RESP_ERR_INVALID_RESOURCE_ID,
        VIRTIO_GPU_RESP_ERR_INVALID_CONTEXT_ID,
        VIRTIO_GPU_RESP_ERR_INVALID_PARAMETER,
    }; // enum class CommandType : u32 

    struct ControlHeader 
    {
        u32 type;
        u32 flags;
        u64 fence_id;
        u32 context_id;
        u32 padding;
    }; // struct ContorlHeader

    struct Rect 
    {
        u32 x;
        u32 y;
        u32 width;
        u32 height;
    }; // struct Rect

    struct DisplayInfoResponse 
    {
        ControlHeader header;
        struct Display 
        {
            Rect rect;
            u32 enabled;
            u32 flags;
        } scanout_modes[VIRTIO_GPU_MAX_SCANOUTS];
    }; // struct DisplayInfoResponse

    enum class TextureFormat : u32 
    {
        VIRTIO_GPU_FORMAT_B8G8R8A8_UNORM = 1,
        VIRTIO_GPU_FORMAT_B8G8R8X8_UNORM = 2,
        VIRTIO_GPU_FORMAT_A8R8G8B8_UNORM = 3,
        VIRTIO_GPU_FORMAT_X8R8G8B8_UNORM = 4,

        VIRTIO_GPU_FORMAT_R8G8B8A8_UNORM = 67,
        VIRTIO_GPU_FORMAT_X8B8G8R8_UNORM = 68,

        VIRTIO_GPU_FORMAT_A8B8G8R8_UNORM = 121,
        VIRTIO_GPU_FORMAT_R8G8B8X8_UNORM = 134,
    }; // enum class TextureFormat : u32

    struct ResourceCreate2D 
    {
        ControlHeader header;
        u32 resource_id;
        u32 format;
        u32 width;
        u32 height;
    }; // struct ResourceCreate2D

    struct ResourceUnref 
    {
        ControlHeader header;
        u32 resource_id;
        u32 padding;
    }; // struct ResourceUnref

    struct SetScanOut 
    {
        ControlHeader header;
        Rect rect;
        u32 scanout_id;
        u32 resource_id;
    }; // struct SetScanOut

    struct MemoryEntry 
    {
        u64 address;
        u32 length;
        u32 padding;
    }; // struct MemoryEntry

    struct ResourceAttachBacking 
    {
        ControlHeader header;
        u32 resource_id;
        u32 num_entries;
    }; // struct ResourceAttachBacking

    struct ResourceDetachBacking 
    {
        ControlHeader header;
        u32 resource_id;
        u32 padding;
    }; // struct ResourceDetachBacking

    struct TransferToHost2D 
    {
        ControlHeader header;
        Rect rect;
        u64 offset;
        u32 resource_id;
        u32 padding;
    }; // struct TransferToHost2D

    struct ResourceFlush 
    {
        ControlHeader header;
        Rect rect;
        u32 resource_id;
        u32 padding;
    }; // struct ResourceFlush

    struct GetEDID 
    {
        ControlHeader header;
        u32 scanout_id;
        u32 padding;
    }; // struct GetEDID

    struct GetEDIDResponse 
    {
        ControlHeader header;
        u32 size;
        u32 padding;
        u8 edid[1024];
    }; // struct GetEDIDResponse

} // namespace Kernel::Graphics::VirtIOGPU::Protocol