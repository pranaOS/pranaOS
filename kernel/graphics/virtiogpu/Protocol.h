/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#define VIRTIO_GPU_MAX_SCANOUTS 16

namespace Kernel::Graphics::VirtIOGPU::Protocol {

enum class CommandType : u32 {
    
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
};

struct ControlHeader {
    u32 type;
    u32 flags;
    u64 fence_id;
    u32 context_id;
    u32 padding;
};

struct Rect {
    u32 x;
    u32 y;
    u32 width;
    u32 height;
};

struct DisplayInfoResponse {
    ControlHeader header;

    struct Display {
        Rect rect;
        u32 enabled;
        u32 flags;
    } scanout_modes[VIRTIO_GPU_MAX_SCANOUTS];
};

enum class TextureFormat : u32 {
    VIRTIO_GPU_FORMAT_B8G8R8A8_UNORM = 1,
    VIRTIO_GPU_FORMAT_B8G8R8X8_UNORM = 2,
    VIRTIO_GPU_FORMAT_A8R8G8B8_UNORM = 3,
    VIRTIO_GPU_FORMAT_X8R8G8B8_UNORM = 4,

    VIRTIO_GPU_FORMAT_R8G8B8A8_UNORM = 67,
    VIRTIO_GPU_FORMAT_X8B8G8R8_UNORM = 68,

    VIRTIO_GPU_FORMAT_A8B8G8R8_UNORM = 121,
    VIRTIO_GPU_FORMAT_R8G8B8X8_UNORM = 134,
};

struct ResourceCreate2D {
    ControlHeader header;
    u32 resource_id;
    u32 format;
    u32 width;
    u32 height;
};

struct ResourceUnref {
    ControlHeader header;
    u32 resource_id;
    u32 padding;
};

struct SetScanOut {
    ControlHeader header;
    Rect rect;
    u32 scanout_id;
    u32 resource_id;
};

struct MemoryEntry {
    u64 address;
    u32 length;
    u32 padding;
};

struct ResourceAttachBacking {
    ControlHeader header;
    u32 resource_id;
    u32 num_entries;
};

struct ResourceDetachBacking {
    ControlHeader header;
    u32 resource_id;
    u32 padding;
};

struct TransferToHost2D {
    ControlHeader header;
    Rect rect;
    u64 offset;
    u32 resource_id;
    u32 padding;
};

struct ResourceFlush {
    ControlHeader header;
    Rect rect;
    u32 resource_id;
    u32 padding;
};

}