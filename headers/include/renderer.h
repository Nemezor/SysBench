#ifndef UI_RENDERER_H
#define UI_RENDERER_H

#include <text_device.h>
#include <stdint.h>

struct renderer_menu {
    void (*render)(void);
    void (*input)(void);
};

struct text_device* renderer_init();
void renderer_switch();
void renderer_repaint();
void renderer_menu(struct renderer_menu* menu);

void renderer_string(uint32_t x, uint32_t y, uint32_t box_width, uint32_t box_height, char* format, ...);
void renderer_set(uint32_t x, uint32_t y, char c);
void renderer_outline(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

#endif
