#ifndef GFX_SOFTWARE_SW_RENDER_H
#define GFX_SOFTWARE_SW_RENDER_H

#include <stdint.h>
#include <gfx_device.h>

void swrender_initialize(uint8_t bsp_id, uint32_t width, uint32_t height);
void swrender_update_screen(uint32_t* destination);
struct gfx_render* swrender_get_gfx_render();
void swrender_run(uint32_t count, void (shader)(uint32_t id, void* uniform_data), void* uniform_data);
void swrender_internal_fragment_shader(float x, float y, float z, float r, float g, float b, float a, float u, float v, float nx, float ny, float nz);
extern void swrender_framebuffercpy(void* back, void* front, void* destination, uint64_t size);

#endif
