/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <libutils/Array.h>
#include <libutils/Lock.h>
#include "ps2/LegacyDevice.h"

struct LegacyATA : public LegacyDevice
{
private:
    Lock _buffer_lock{"legacy-ata"};

    void identify();
    void select();
    void has_failed(uint8_t status);

    void delay(uint16_t io_port);
    void poll(uint16_t io_port);

    
    void write_lba(uint16_t io_port, uint32_t lba);
    uint8_t read_block(uint8_t *buf, uint32_t lba, size_t size);
    uint8_t write_block(uint8_t *buf, uint32_t lba, size_t size);

    int _bus;
    int _drive;
    Array<uint16_t, 256> _ide_buffer;
    bool _exists = false;
    String _model;
    bool _supports_48lba;
    size_t _num_blocks;

};