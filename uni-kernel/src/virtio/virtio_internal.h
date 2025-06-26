/**
 * @file virtio_internal.h
 * @author Krisna Pranav
 * @brief virtio internal
 * @version 6.0
 * @date 2025-06-26
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#define VIRTIO_ID_NETWORK       1
#define VIRTIO_ID_BLOCK         2
#define VIRTIO_ID_CONSOLE       3
#define VIRTIO_ID_ENTROPY       4
#define VIRTIO_ID_BALLOON       5
#define VIRTIO_ID_IOMEMORY      6
#define VIRTIO_ID_RPMSG         7
#define VIRTIO_ID_SCSI          8
#define VIRTIO_ID_9P            9
#define VIRTIO_ID_RPROC_SERIAL  11
#define VIRTIO_ID_CAIF          12
#define VIRTIO_ID_GPU           16
#define VIRTIO_ID_INPUT         18
#define VIRTIO_ID_VSOCK         19
#define VIRTIO_ID_CRYPTO        20

typedef struct virtqueue *virtqueue;

/**
 * @brief Construct a new closure type object
 * 
 * @param len 
 */
closure_type(vqfinish, void, u64 len);

#define VIRTIO_CONFIG_STATUS_RESET	0x00
#define VIRTIO_CONFIG_STATUS_ACK	0x01
#define VIRTIO_CONFIG_STATUS_DRIVER	0x02
#define VIRTIO_CONFIG_STATUS_DRIVER_OK	0x04
#define VIRTIO_CONFIG_STATUS_FEATURE	0x08
#define VIRTIO_CONFIG_STATUS_FAILED	0x80
#define VIRTIO_F_NOTIFY_ON_EMPTY U64_FROM_BIT(24)
#define VIRTIO_F_ANY_LAYOUT     U64_FROM_BIT(27)
#define VIRTIO_F_RING_INDIRECT_DESC	U64_FROM_BIT(28)
#define VIRTIO_F_RING_EVENT_IDX		U64_FROM_BIT(29)
#define VIRTIO_F_BAD_FEATURE U64_FROM_BIT(30)
#define VIRTIO_TRANSPORT_F_START	28
#define VIRTIO_TRANSPORT_F_END		32
#define VIRTIO_F_VERSION_1 U64_FROM_BIT(32)

/**
 * @brief Construct a new closure type object
 * 
 * @param queue_index 
 * @param notify_offset 
 */
closure_type(vtdev_notify, void, u16 queue_index, bytes notify_offset);

typedef struct vtdev {
    u64 dev_features;              
    u64 features;                  

    backed_heap contiguous;
    heap general;

    enum vtio_transport {
        VTIO_TRANSPORT_MMIO,
        VTIO_TRANSPORT_PCI,
    } transport;

    vtdev_notify notify;
} *vtdev;

/**
 * @param dev 
 * @param offset 
 * @return u8 
 */
u8 vtdev_cfg_read_1(vtdev dev, u64 offset);

/**
 * @param dev 
 * @param offset 
 * @return u16 
 */
u16 vtdev_cfg_read_2(vtdev dev, u64 offset);

/**
 * @param dev 
 * @param offset 
 * @return u32 
 */
u32 vtdev_cfg_read_4(vtdev dev, u64 offset);

/**
 * @param dev 
 * @param offset 
 * @param value 
 */
void vtdev_cfg_write_1(vtdev dev, u64 offset, u8 value);

/**
 * @param dev 
 * @param offset 
 * @param value 
 */
void vtdev_cfg_write_2(vtdev dev, u64 offset, u16 value);

/**
 * @param dev 
 * @param offset 
 * @param value 
 */
void vtdev_cfg_write_4(vtdev dev, u64 offset, u32 value);

/**
 * @param dev 
 * @param offset 
 * @param dest 
 * @param len 
 */
void vtdev_cfg_read_mem(vtdev dev, u64 offset, void *dest, bytes len);

/**
 * @param dev 
 * @param status 
 */
void vtdev_set_status(vtdev dev, u8 status);

/**
 * @param dev 
 * @return boolean 
 */
static inline boolean vtdev_is_modern(vtdev dev)
{
    return (dev->features & VIRTIO_F_VERSION_1) != 0;
}

/**
 * @param h 
 * @param page_allocator 
 * @param transport 
 * @param d 
 */
static inline void virtio_attach(heap h, backed_heap page_allocator, enum vtio_transport transport, vtdev d)
{
    d->general = h;
    d->contiguous = page_allocator;
    d->transport = transport;
}

/**
 * @param dev 
 * @param name 
 * @param idx 
 * @param cpu_affinity 
 * @param result 
 * @return status 
 */
status virtio_alloc_vq_aff(vtdev dev, sstring name, int idx, range cpu_affinity, struct virtqueue **result);

#define virtio_alloc_virtqueue(dev, name, idx, result)  \
    virtio_alloc_vq_aff(dev, name, idx, irange(0, 0), result)

/**
 * @param dev 
 * @param handler 
 * @return status 
 */
status virtio_register_config_change_handler(vtdev dev, thunk handler);

/**
 * @param dev 
 * @param name 
 * @param queue_index 
 * @param size 
 * @param notify_offset 
 * @param align 
 * @param vqp 
 * @param t 
 * @return status 
 */
status virtqueue_alloc(vtdev dev, sstring name, u16 queue_index, u16 size, bytes notify_offset, int align, struct virtqueue **vqp, thunk *t);

#define VRING_USED_F_NO_NOTIFY  1

#define VRING_AVAIL_F_NO_INTERRUPT      1

/**
 * @param vq 
 * @return physical 
 */
physical virtqueue_desc_paddr(struct virtqueue *vq);

/**
 * @param vq 
 * @return physical 
 */
physical virtqueue_avail_paddr(struct virtqueue *vq);

/**
 * @param vq 
 * @return physical 
 */
physical virtqueue_used_paddr(struct virtqueue *vq);

/**
 * @param vq 
 * @return u16 
 */
u16 virtqueue_entries(virtqueue vq);

/**
 * @param vq 
 * @return u16 
 */
u16 virtqueue_free_entries(virtqueue vq);

/**
 * @param vq 
 * @param enable 
 */
void virtqueue_set_polling(virtqueue vq, boolean enable);

typedef struct vqmsg *vqmsg;

/**
 * @param vq 
 * @return vqmsg 
 */
vqmsg allocate_vqmsg(virtqueue vq);

/**
 * @param vq 
 * @param m 
 */
void deallocate_vqmsg(virtqueue vq, vqmsg m);

/**
 * @param vq 
 * @param m 
 * @param phys_addr 
 * @param len 
 * @param write 
 */
void vqmsg_push(virtqueue vq, vqmsg m, u64 phys_addr, u32 len, boolean write);

/**
 * @param vq 
 * @param m 
 * @param completion 
 * @param seqno 
 * @param kick 
 */
void vqmsg_commit_seqno(virtqueue vq, vqmsg m, vqfinish completion, u32 *seqno, boolean kick);

#define vqmsg_commit(vq, m, completion) vqmsg_commit_seqno(vq, m, completion, 0, true)

/**
 * @param vq 
 */
void virtqueue_kick(virtqueue vq);