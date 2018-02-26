#ifndef GFX_GFX_DEVICE_H
#define GFX_GFX_DEVICE_H

#include <stdint.h>
#include <math.h>
#include <render/textures.h>

struct gfx_render {
    uint32_t (*get_width)(void);
    uint32_t (*get_height)(void);
    void (*set_projection)(matrix_t* matrix);
    void (*set_transformation)(matrix_t* matrix);
    void (*draw_pixel)(uint32_t x, uint32_t y, uint32_t color);
    void (*draw_triangles)(uint32_t triangle_count, float* vertices, float* uvs, float* colors, float* normals);
    void (*set_viewport)(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
    void (*clear)(uint32_t color, float depth);
    uint32_t (*texture_sample)(uint8_t index, float u, float v);
    void (*texture_bind)(uint8_t index, texture_t* texture);
};

struct gfx_device {
    void (*update_screen)(void);
    struct gfx_render* renderer;
};

#endif
