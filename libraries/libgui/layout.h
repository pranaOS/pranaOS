#ifndef GUI_LAYOUT_H
#define GUI_LAYOUT_H

#include <libcore/hash.h>
#include <libgui/event.h>
#include <libgui/framebuffer.h>
#include <libgui/msgui.h>
#include <list.h>
#include <poll.h>
#include <stdint.h>
#include <sys/cdefs.h>

struct window;

typedef void (*EVENT_HANDLER)(struct window *win);

struct graphic {
    char *buf;
    int32_t x, y;
    uint16_t width, height;
    bool transparent;
};

struct icon {
    char *label;
    char *exec_path;
    char *icon_path;

    bool active;
};

int get_character_width(char ch);
int get_character_height(char ch);
void init_fonts();

#endif 