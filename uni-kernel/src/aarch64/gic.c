/**
 * @file gic.c
 * @author Krisna Pranav
 * @brief gic
 * @version 6.0
 * @date 2025-03-29
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

#define GIC_LPI_ENABLE 0x01

#ifndef GIC_DEBUG
#define gic_debug(x, ...)
#else
#define gic_debug(x, ...)
#endif

#define GIC_CMD_QUEUE_SIZE (64 * KB)

static struct
{
    boolean v3_iface;
    u32 intid_mask;
    u64 dist_base;
    struct
    {
        u64 base;
        u64 propbase;
        u64 pendbase;
    } redist;

    u64 its_phys_base;
    u64 its_base;
} gic;

typedef struct its_dev
{
    struct list l;
    u32 id;
    void* itt;
}* its_dev;

#define gicd_read_32(reg)

void gic_enable(int irq)
{
}

void gic_clear_pending_int(int req)
{
    if(irq < GIC_LPI_ENABLE)
    {
    }
}