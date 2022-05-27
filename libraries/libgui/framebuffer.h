#ifndef GUI_FRAMEBUFFER_H
#define GUI_FRAMEBUFFER_H

#include <stdint.h>

struct framebuffer {
    uint32_t addr;
    uint32_t pitch;
    uint32_t width;
    uint32_t height;
    uint8_t bpp;
};

#endif