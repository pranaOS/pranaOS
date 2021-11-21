/**
 * @file boot64main.c
 * @version 0.1
 * @date 2021-11-21
 * 
 * @copyright Copyright (c) 2021 Krisna Pranav
 * 
 */

typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long uint64;

typedef void (*go_entry_t)(uint32, uint32);

void boot64main(uint32 gomain, uint32 magic, uint32 mbinfo) {
    go_entry_t go_entry;
    
    go_entry = (go_entry_t)((uint64)gomain);
    go_entry(magic, mbinfo);

    for (;;) {

    }
}