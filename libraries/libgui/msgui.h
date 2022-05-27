#ifndef GUI_MSGUI_H
#define GUI_MSGUI_H

#include <stdbool.h>
#include <stdint.h>

#define WINDOW_NAME_LENGTH 7
#define WINDOW_SERVER_QUEUE "window_server"

struct msgui_window {
    int32_t x, y;
    uint32_t width, height;
    bool transparent;
    char parent[WINDOW_NAME_LENGTH];
    char sender[WINDOW_NAME_LENGTH];
};

struct msgui_focus {
    char sender[WINDOW_NAME_LENGTH];  
};

struct msgui_render {
    char sender[WINDOW_NAME_LENGTH];
};

#endif