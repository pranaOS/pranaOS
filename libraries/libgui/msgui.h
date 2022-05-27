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

struct msgui_close {
	char sender[WINDOW_NAME_LENGTH];
};

enum msgui_type {
	MSGUI_WINDOW,
	MSGUI_RENDER,
	MSGUI_FOCUS,
	MSGUI_CLOSE,
};

struct msgui {
	enum msgui_type type;
	char data[128];
};

#endif
