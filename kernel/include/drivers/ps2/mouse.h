#pragma once

#include <stdint.h>

typedef void (*mousehook_fpointer)(int16_t xrel, int16_t yrel, uint16_t xloc,
                                   uint16_t yloc, int lbut, int rbut, int mbut);

int     init_mouse();
void    mouse_poll();
uint8_t register_mouse_hook(mousehook_fpointer hook);
uint8_t remove_mouse_hook(mousehook_fpointer hook);
void    mouse_loop();

#endif