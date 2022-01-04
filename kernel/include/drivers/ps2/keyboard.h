#pragma once

#include <stdint.h>

int init_ps2_keyboard();

void register_keyboard_hook(keyhook_fpointer hook);

void remove_keyboard_hook();