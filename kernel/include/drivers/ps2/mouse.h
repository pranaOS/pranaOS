#pragma once 

#include <stdint.h>


typedef void (*mousehook_fpointer)(int16_t xrel, int16_t yrel, uint16_t xloc,
                                   uint16_t yloc, int lbut, int rbut, int mbut);

int init_mouse();
void mouse_poll();
void mouse_loop();