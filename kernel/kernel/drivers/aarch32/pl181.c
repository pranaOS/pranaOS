/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <drivers/aarch32/pl181.h>
#include <libkernel/log.h>
#include <mem/vmm/vmm.h>
#include <mem/vmm/zoner.h>

static sd_card_t sd_cards[MAX_DEVICES_COUNT];
static zone_t mapped_zone;
static volatile pl181_registers_t* registers = (pl181_registers_t*)PL181_BASE;

static inline int _pl181_map_itself()
{
    mapped_zone = zoner_new_zone(sizeof(pl181_registers_t));
    vmm_map_page(mapped_zone.start, PL181_BASE, PAGE_READABLE | PAGE_WRITABLE | PAGE_EXECUTABLE);
    registers = (pl181_registers_t*)mapped_zone.ptr;
    return 0;

}

static int _pl181_write_block(device_t* device, uint32_t lba_like, void* write_data)
{
    sd_card_t* sd_card = &sd_cards[device->id];
    uint32_t* write_data = (uint32_t*)write_data;
    uint32_t bytes_written = 0;

    registers->data_length = PL181_SECTOR_SIZE;
    registers->data_control = 0b01;

    if (sd_card->ishc) {
        _pl181_send_cmd(CMD_WRITE_SINGLE_BLOCK | MMC_CMD_ENABLE_MASK | MMC_CMD_RESP_MASK, lba_like);
    } else {
        _pl181_send_cmd(CMD_WRITE_SINGLE_BLOCK | MMC_CMD_ENABLE_MASK | MMC_CMD_RESP_MASK, lba_like * PL181_SECTOR_SIZE);

    }

    while (regsiters->status & MMC_STAT_TRANSMIT_FIFO_EMPTY_MASK) {
        registers->fifo_data[0] = *write_data32;
        write_data32++;
        bytes_written += 4;
    }
    return bytes_written;
}

static uint32_t _pl181_get_capacity(device_t* device)
{
    sd_card_t* sd_card = &sd_cards[device->id];
    return sd_card->capacity;
}