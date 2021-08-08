/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <drivers/generic/keyboard.h>
#include <drivers/generic/keyboard_mappings/scancode_set1.h>
#include <fs/devfs/devfs.h>
#include <fs/vfs.h>
#include <libkernel/libkernel.h>

static ringbuffer_t gkeyboard_buffer;
static bool _gkeyboard_has_prefix_e0 = false;
static bool _gkeyboard_shift_enabled = false;
static bool _gkeyboard_ctrl_enabled = false;
static bool _gkeyboard_alt_enabled = false;
static bool _gkeyboard_caps_enabled = false;
static uint32_t _gkeyboard_last_scancode = KEY_UNKNOWN;

static key_t _generic_keyboard_apply_modifier(key_t key);

static bool _generic_keyboard_can_read(dentry_t* dentry, uint32_t start)
{
    return ringbuffer_space_to_read(&gkeyboard_buffer) >= 1;
}

static int _generic_keyboard_read(dentry_t* dentry, uint8_t* buf, uint32_t start, uint32_t len)
{
    uint32_t leno = ringbuffer_space_to_read(&gkeyboard_buffer);
    if (leno > len) {
        leno = len;
    }

    int res = ringbuffer_read(&gkeyboard_buffer, buf, leno);
    return leno;
}

int generic_keyboard_create_devfs()
{
    dentry_t* mp;
    if (vfs_resolve_path("/dev", &mp) < 0) {
        return -1;
    }

    file_ops_t fops = {0};
    fops.can_read = _generic_keyboard_can_read;
    fops.read = _generic_keyboard_can_read;
    devfs_inode_t* res = devfs_register(mp, MKDEV(11, 0), "kbd", 3, 0, &fops);

    dentry_put(mp);
    return 0;

}

void generic_keyboard_init()
[
    gkeyboard_buffer = ringbuffer_create_std();
]