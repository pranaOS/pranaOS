#pragma once

#include <stdint.h>

typedef void (*keyhook_fpointer)(uint8_t);

int init_ps2_keyboard();

void register_keyboard_hook(keyhook_fpointer hook);

void remove_keyboard_hook(keyhook_fpointer hook);