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
	struct graphic icon_graphic;
	struct graphic box_graphic;
	bool active;
};

struct ui_style {
	int32_t padding_top, padding_left, padding_right, padding_bottom;
};

struct ui_mouse {
	struct graphic graphic;
	uint8_t buttons;
};

struct desktop {
	struct graphic graphic;
	struct ui_mouse mouse;
	struct framebuffer *fb;
	struct window *active_window;
	struct list_head children;
	struct hashmap icons;
	unsigned int event_state;
};

#define WINDOW_EVENT_CLICK "click"

struct window {
	char name[WINDOW_NAME_LENGTH];
	struct graphic graphic;
	struct window *parent;
	struct window *active_window;
	struct ui_style *style;
	struct list_head sibling;
	struct list_head children;
	struct hashmap events;
	void (*add_event_listener)(struct window *win, char *event_name, EVENT_HANDLER handler);
};

struct ui_label {
	struct window window;
	char *text;
	void (*set_text)(struct ui_label *label, char *text);
};

struct ui_input {
	struct window window;
	char *value;
};

struct ui_button {
	struct window window;
	char *icon;
};

struct ui_block {
	struct window window;
};

/**
 * @brief Get the character width object
 * 
 * @param ch 
 * @return int 
 */
int get_character_width(char ch);

/**
 * @brief Get the character height object
 * 
 * @param ch 
 * @return int 
 */
int get_character_height(char ch);

/**
 * @brief gui draw rectangle
 * 
 * @param win 
 * @param x 
 * @param y 
 * @param width 
 * @param height 
 * @param bg 
 */
void gui_draw_retangle(struct window *win, int x, int y, unsigned int width, unsigned int height, uint32_t bg);

/**
 * @brief gui create label
 * 
 * @param parent 
 * @param label 
 * @param x 
 * @param y 
 * @param width 
 * @param height 
 * @param text 
 * @param padding 
 */
void gui_create_label(struct window *parent, struct ui_label *label, int32_t x, int32_t y, uint32_t width, uint32_t height, char *text, struct ui_style *padding);

/**
 * @brief gui create input
 * 
 * @param parent 
 * @param input 
 * @param x 
 * @param y 
 * @param width 
 * @param height 
 * @param content 
 */
void gui_create_input(struct window *parent, struct ui_input *input, int32_t x, int32_t y, uint32_t width, uint32_t height, char *content);

/**
 * @brief gui create button
 * 
 * @param parent 
 * @param button 
 * @param x 
 * @param y 
 * @param width 
 * @param height 
 * @param transparent 
 * @param style 
 */
void gui_create_button(struct window *parent, struct ui_button *button, int32_t x, int32_t y, uint32_t width, uint32_t height, bool transparent, struct ui_style *style);

/**
 * @brief gui create block
 * 
 * @param parent 
 * @param block 
 * @param x 
 * @param y 
 * @param width 
 * @param height 
 * @param transparent 
 * @param style 
 */
void gui_create_block(struct window *parent, struct ui_block *block, int32_t x, int32_t y, uint32_t width, uint32_t height, bool transparent, struct ui_style *style);

/**
 * @brief gui render
 * 
 * @param win 
 */
void gui_render(struct window *win);

/**
 * @brief init window
 * 
 * @param x 
 * @param y 
 * @param width 
 * @param height 
 * @return struct window* 
 */
struct window *init_window(int32_t x, int32_t y, uint32_t width, uint32_t height);

/**
 * @brief load bmp
 * 
 * @param path 
 * @return char* 
 */
char *load_bmp(char *path);

/**
 * @brief eneter event loop
 * 
 * @param win 
 * @param event_callback 
 * @param fds 
 * @param nfds 
 * @param fds_callback 
 */
void enter_event_loop(struct window *win, void (*event_callback)(struct xevent *evt), int *fds, unsigned int nfds, void (*fds_callback)(struct pollfd *, unsigned int));

/**
 * @brief init fonts
 * 
 */
void init_fonts();

/**
 * @brief Set the pixel object
 * 
 * @param pixel_dest 
 * @param red 
 * @param green 
 * @param blue 
 * @param alpha_raw 
 * @return __inline 
 */
static __inline void set_pixel(char *pixel_dest, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha_raw) {
	uint8_t red_dest = pixel_dest[0];
	uint8_t green_dest = pixel_dest[1];
	uint8_t blue_dest = pixel_dest[2];
	uint8_t alpha_raw_dest = pixel_dest[3];

	float alpha = alpha_raw / (float)255;
	float alpha_dest = alpha_raw_dest / (float)255;

	float adj = (1 - alpha) * alpha_dest;
	pixel_dest[0] = red * alpha + adj * red_dest;
	pixel_dest[1] = green * alpha + adj * green_dest;
	pixel_dest[2] = blue * alpha + adj * blue_dest;
	pixel_dest[3] = (alpha + adj) * 255;
}

#endif
